#include<iostream>
#include<conio.h>
#include<vector>
#include<fstream>
using namespace std;
class Network {
	struct Computer {
		int id;
		Computer * next;
		//method to enable if(n[i][j]) cout<<"i and j are connected.";
		bool operator[] (int j)
		{
			bool flag = 0;
			Computer*ptr = this;
			ptr = ptr->next;
			while (ptr != nullptr && !flag)
			{
				if (ptr->id == j)
					flag = 1;
				ptr = ptr->next;
			}
			return flag;
		}
		void addConnection(Computer*&head, int id)
		{
			Computer*ptr;
			if (head != nullptr)
			{
				ptr = head;
				while (ptr->next != nullptr)
				{
					ptr = ptr->next;
				}
				ptr->next = new Computer;
				ptr = ptr->next;
			}
			else
			{

				head = new Computer;
				ptr = head;
			}

			ptr->id = id;
			ptr->next = nullptr;
		}
		const Computer&operator=(Computer*&head) {
			Computer*nextptr;
			Computer*temp = this;
			while (temp != nullptr)
			{
				nextptr = temp->next;
				temp->next = nullptr;
				delete[]temp;
				temp = nextptr;
			}
			Computer*curr = head;
			Computer*thisptr = this;
			while (curr != nullptr)
			{
				Computer::addConnection(thisptr, curr->id);
				curr = curr->next;
			}
			return *this;
		}

	};
	vector<Computer*> net;
	//add id into the list pointed to by head

public:
	void addConnection(Computer*&head, int id);
	//for empty network
	Network();
	//read a network from a file
	Network(string filename);
	//deep copy methods
	Network(const Network& obj);
	const Network& operator=(const Network& obj);
	//create net array of size, with no connections
	Network(int size);
	//connect computers x and y
	//use the utility method addConnection
	void addConnection(int x, int y);
	//merge two networks (take union)
	//computers, connections in any one of the networks appear in result
	Network operator + (const Network& obj);
	//intersect two networks (extract the common core)
	//links and computers present in both networks appear in the result
	Network operator * (const Network& obj);
	//Remove the common connections of obj and this network
	Network operator - (const Network& obj);
	//Take complement of the Network
	//Returns a network with the same computers
	//but which contains complementary connections
	//resultant contains connections which are absent in this network
	Network operator- ();
	//print the network
	friend ostream & operator << (ostream & out, const Network& obj);
	//method to enable if(n[i][j]){cout<<"i and j are connected.";}
	Computer & operator[] (int i);
	//add another computer to the network
	Network operator++ (int);
	//logical methods
	//subNetwork returns true if obj is a sub-network of this network
	bool subNetwork(Network& obj);
	//get all neighbors of computer nid
	vector<int> getNeighbors(int nid);
	//get all unique neighbors-of-neighbors of computer nid
	vector<int> getNeighborsOfNeighbors(int nid);
	//returns all computers in order of their number of neighbors
	//computer with most neighbors comes first and so on
	vector<int> orderOfDegree();
	//Suggest connection
	//Returns two unconnected computers with most common neighbors
	vector <int> suggestConnection();
	vector<int>fill();
	//de-allocate network
	~Network();
};
Network::Network() {
	net.empty();
}
Network::Network(string filename) {
	fstream fin;
	fin.open(filename);
	if (fin.is_open())
	{
		char junk = '1';
		int i = 0;
		Computer*temp;
		while (!fin.eof())
		{
			net.resize(i + 1);
			temp = net[i] = new Computer;
			do {
				temp->id = fin.get() - '0';
				junk = fin.get();

				//				cout << temp->id << "\t";
				if (junk != '\n' && junk != EOF)
					temp->next = new Computer;
				else
					temp->next = nullptr;
				temp = temp->next;
			} while (junk != '\n' && junk != EOF);
			i++;
			//		cout<<endl;
		}

	}
	else
		cout << "corrupted\n";
}
Network::Network(int size) {
	net.resize(size);
	for (int i = 0; i < net.size(); i++)
		net[i] = nullptr;
}
Network::Network(const Network&obj) {
	net = obj.net;
	for (int i = 0; i < obj.net.size(); i++)
	{
		net[i] = nullptr;
		Computer*curr = obj.net[i];
		while (curr != nullptr)
		{
			addConnection(net[i], curr->id);
			curr = curr->next;
		}
	}
}
void Network::addConnection(int x, int y)
{
	if (x >= net.size())
		net.resize(x+1);
	if (y >= net.size())
		net.resize(y + 1);
	net[net.size() - 1] = nullptr;
	addConnection(net[x], y);
	addConnection(net[y], x);
}
void Network::addConnection(Computer*&head, int id)
{
	Computer*ptr;
	if (head != nullptr)
	{
		ptr = head;
		while (ptr->next != nullptr)
		{
			ptr = ptr->next;
		}
		ptr->next = new Computer;
		ptr = ptr->next;
	}
	else
	{
		head = new Computer;
		ptr = head;
	}
	ptr->id = id;
	ptr->next = nullptr;
}
const Network& Network::operator=(const Network& obj)
{
	if (net.size() != 0)
	{
		Computer*temp;
		Computer*nextptr;
		for (int i = 0; i < net.size(); i++)
		{
			temp = net[i];
			while (temp != nullptr)
			{
				nextptr = temp->next;
				temp->next = nullptr;
				delete[]temp;
				temp = nextptr;
			}
		}
		net.~vector();
	}
	net = obj.net;
	for (int i = 0; i < obj.net.size(); i++)
	{
		net[i] = nullptr;
		Computer*curr = obj.net[i];
		while (curr != nullptr)
		{
			addConnection(net[i], curr->id);
			curr = curr->next;
		}
	}
	return *this;
}
Network::~Network() {
	if (net.size() != 0)
	{
		Computer*temp;
		Computer*nextptr;
		for (int i = 0; i < net.size(); i++)
		{
			temp = net[i];
			while (temp != nullptr)
			{
				nextptr = temp->next;
				temp->next = nullptr;
				delete[]temp;
				temp = nextptr;
			}
		}
	}
	cout << "\ndestructor called\n";
}
ostream  &operator<<(ostream&out, const Network& obj)
{
	Network::Computer*ptr;
	for (int i = 0; i < obj.net.size(); i++)
	{
		ptr = obj.net[i];
		while (ptr != nullptr)
		{
			out << ptr->id << "\t";
			ptr = ptr->next;
		}
		out << endl;
	}
	return out;
}
Network Network::operator+(const Network&obj)
{
	int first;
	Computer*ptr = nullptr, *ptr2 = nullptr, *ptr3 = nullptr;
	Network ans;
	ans.net.empty();
	if (net.size() == 0) {
		ans = obj;
	}
	else if (obj.net.size() == 0) {
		ans = *this;
	}
	else
	{
		int flag = 0;
		//cout << "here\n";
		if (net.size() > obj.net.size())
		{
			first = 1;
			ans = *this;
		}
		else
		{
			first = 0;
			ans = obj;
		}
		for (int i = 0; i < ans.net.size(); i++)
		{
			if (i < obj.net.size() && i < net.size())
			{
				if (obj.net[i] != nullptr && net[i] != nullptr)
				{
					if (first)
					{
						ptr = net[i]->next;
						ptr2 = obj.net[i]->next;
					}
					else {
						ptr = obj.net[i]->next;
						ptr2 = net[i]->next;
					}
					cout << "index:" << i << endl;
					while (ptr2 != nullptr)
					{
						//	cout <<ptr2->id << endl;
						flag = 0;
						if (first)
						{
							ptr = net[i]->next;
							//ptr2 = obj.net[i]->next;
						}
						else {
							ptr = obj.net[i]->next;
							//ptr2 = net[i]->next;
						}
						while (ptr != nullptr)
						{
							//	cout << ptr->id << ",";
							if (ptr2->id == ptr->id)
								flag = 1;
							ptr = ptr->next;
						}
						//cout << endl << endl;
						if (flag == 0) {
							//cout << "\nconnecting" << ans.net[i]->id << "," << ptr2->id << endl;
							ans.addConnection(ans.net[i], ptr2->id);
						}
						ptr2 = ptr2->next;
					}
				}
			}
		}
	}
	cout << endl << endl << ans;
	return ans;
}
Network Network::operator-(const Network&obj)
{
	int first;
	Computer*ptr = nullptr, *ptr2 = nullptr, *ptr3 = nullptr;
	Network ans;
	ans.net.empty();
	if (net.size() == 0) {
		ans.net.empty();
	}
	else if (obj.net.size() == 0) {
		ans = *this;
	}
	else
	{
		ans.net.resize(net.size());
		int flag = 0;
		//cout << "here\n";
		for (int i = 0; i < ans.net.size(); i++)
		{
			if (i >= obj.net.size() || obj.net[i]->next == nullptr)
			{
				ptr = net[i]->next;
				while (ptr != nullptr)
				{
					if (ans.net[i] == nullptr)
						ans.addConnection(ans.net[i], net[i]->id);
					ans.addConnection(ans.net[i], ptr->id);
					ptr = ptr->next;
				}
			}
			else
			{
				ptr = net[i]->next;
				ptr2 = obj.net[i]->next;
				while (ptr != nullptr)
				{
					//cout << ptr->id << ",";
					flag = 0;
					ptr2 = obj.net[i]->next;
					while (ptr2 != nullptr)
					{
						if (ptr->id == ptr2->id)
							flag = 1;
						ptr2 = ptr2->next;
					}
					if (flag == 0)
					{
						if (ans.net[i] == nullptr)
							ans.addConnection(ans.net[i], i);
						ans.addConnection(ans.net[i], ptr->id);
					}
					ptr = ptr->next;
				}
				//cout << endl;
			}

		}
	}
	//cout << endl << endl << ans;
	return ans;
}
Network Network::operator * (const Network& obj)
{
	int first;
	Computer*ptr = nullptr, *ptr2 = nullptr, *ptr3 = nullptr;
	Network ans;
	ans.net.empty();
	if (net.size() == 0 && obj.net.size() == 0) {
		ans.net.empty();
	}
	else if (net.size() == 0 || obj.net.size() == 0) {
		ans.net.empty();
	}
	else
	{
		ans.net.resize(net.size());
		int flag = 0;
		//cout << "here\n";
		for (int i = 0; i < ans.net.size(); i++)
		{
			if (i < obj.net.size() && obj.net[i]->next != nullptr && net[i]->next != nullptr)
			{
				ptr = net[i]->next;
				ptr2 = obj.net[i]->next;
				while (ptr != nullptr)
				{
					//cout << ptr->id << ",";
					flag = 0;
					ptr2 = obj.net[i]->next;
					while (ptr2 != nullptr)
					{
						if (ptr->id == ptr2->id)
							flag = 1;
						ptr2 = ptr2->next;
					}
					if (flag)
					{
						if (ans.net[i] == nullptr)
							ans.addConnection(ans.net[i], i);
						ans.addConnection(ans.net[i], ptr->id);
					}
					ptr = ptr->next;
				}
				//cout << endl;
			}

		}
	}
	//cout << endl << endl << ans;
	return ans;
}
vector<int> Network::getNeighbors(int nid)
{
	vector<int>vec;
	Network::Computer*ptr;
	for (int i = 0; i < net.size(); i++)
	{
		if (net[i]->id == nid)
		{
			ptr = net[i];
			ptr = ptr->next;
			while (ptr != nullptr)
			{
				vec.push_back(ptr->id);
				ptr = ptr->next;
			}
		}
	}
	return vec;
}
vector<int> Network::getNeighborsOfNeighbors(int nid)
{
	vector<int>vec = getNeighbors(nid);
	vector<int>vec2;
	vector<int>res;
	bool flag = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		vec2.empty();
		vec2 = getNeighbors(vec[i]);
		for (int j = 0; j < vec2.size(); j++)
		{
			flag = 0;
			for (int k = 0; k < res.size() && !flag; k++)
			{
				if (res[k] == vec2[j])
					flag = 1;
			}
			if (!flag)
				res.push_back(vec2[j]);
		}
	}
	return res;
}
vector<int> Network::orderOfDegree()
{
	vector<int>size;
	for (int i = 0; i < net.size(); i++)
	{
		size.push_back(getNeighbors(net[i]->id).size());
	}
	vector<int>res;
	for (int i = 0; i < size.size(); i++)
	{
		res.push_back(net[i]->id);
		cout << res[i] << "\t:\t" << size[i] << endl;
	}
	for (int i = 0; i < size.size(); i++)
	{
		for (int j = i + 1; j < size.size(); j++)
		{
			if (size[i] < size[j])
			{
				swap(res[i], res[j]);
				swap(size[i], size[j]);
			}
		}
	}
	return res;
}
vector <int> Network::suggestConnection()
{
	vector<int>id1;
	vector<int>id2;
	vector<int>common;
	Computer*curr = nullptr, *other = nullptr;
	bool connection;
	for (int i = 0; i < net.size(); i++)
	{
		for (int j = i + 1; j < net.size(); j++)
		{

			curr = net[i];
			connection = 0;
			other = net[j];
			other = other->next;
			while (other != nullptr)
			{
				if (curr->id == other->id)
					connection = 1;
				other = other->next;
			}
			if (connection == 0)
			{
				id1.push_back(net[i]->id);
				id2.push_back(net[j]->id);
				curr = net[i]->next;
				curr = curr->next;
				other = net[j]->next;
				other = other->next;
				int count = 0;
				while (curr != nullptr)
				{
					while (other != nullptr)
					{
						if (curr->id == other->id)
							count++;
						other = other->next;
					}
					curr = curr->next;
				}
				common.push_back(count);

			}
		}
	}
	int maxindex = 0;
	int max = common[0];
	for (int j = 1; j < common.size(); j++)
	{
		if (max < common[j])
		{
			max = common[j];
			maxindex = j;
		}
	}
	vector<int>res;
	res.push_back(id1[maxindex]);
	res.push_back(id2[maxindex]);
	return res;

}
Network::Computer& Network::operator[] (int i)
{
	return *net[i];
}
Network Network::operator++ (int)
{
	Computer*obj = new Computer;
	obj->next = nullptr;
	obj->id = net[net.size()-1]->id+1;
	net.push_back(obj);
	Network junk = *this;
	return junk;
}
bool Network::subNetwork(Network& obj)
{
	bool flag=1;
	bool found = 0;
	Computer*curr,*currobj;
	if (net.size() >= obj.net.size())
	{
		for (int i = 0; i < obj.net.size() && flag; i++)
		{
			if (obj.net[i] != nullptr)
			{
				if (net[i] != nullptr)
				{
					flag = 1;
					currobj = obj.net[i]->next;
					while (currobj != nullptr && flag)
					{
						found = 0;
						curr = net[i]->next;
						while (curr != nullptr &&  !found)
						{
							if (curr->id == currobj->id)
								found = 1;
							curr = curr->next;
						}
						if (!found)
							flag = 0;
						currobj = currobj->next;
					}
				}
				else
					flag = 0;
			}
		}
	}
	else
		return false;
	return flag;
}
vector<int>Network::fill()
{
	vector<int>vec;
	vec.empty();
	Network::Computer*ptr;
	for (int i = 0; i < net.size(); i++) {
		ptr = net[i];
		while (ptr != nullptr) {
			vec.push_back(ptr->id);
			ptr = ptr->next;
		}
	}
	return vec;
}
Network Network::operator- ()
{
	bool check = 0;
	Computer*curr = nullptr, *other = nullptr;
	bool connection;
	Network temp(net.size());
	for (int i = 0; i < temp.net.size(); i++)
		temp.net[i] = nullptr;
	for (int i = 0; i < net.size(); i++)
	{
		check = 0;
		for (int j = 0; j < net.size(); j++)
		{
			if (i != j)
			{
				curr = net[i];
				curr = curr->next;
				connection = 0;
				other = net[j];
				other = other->next;
				while (curr != nullptr)
				{
					if (curr->id == net[j]->id)
						connection = 1;
					curr = curr->next;
				}
				if (connection == 0)
				{
					check = 1;
					if (temp.net[net[i]->id] == nullptr)
						temp.addConnection(temp.net[net[i]->id], net[i]->id);
					temp.addConnection(temp.net[net[i]->id], net[j]->id);
				}
				//cout << temp << endl;
			}

		}
		if (check == 0)
			temp.addConnection(temp.net[net[i]->id], net[i]->id);
	}

	return temp;

}
int main()
{
	Network a("a.txt");// ("a.txt");
	Network b("b.txt");
	cout << a<<endl;
	a.addConnection(10,2);
	cout << a;
	///cout << endl << a.subNetwork(b)<<endl;
	system("pause");
}
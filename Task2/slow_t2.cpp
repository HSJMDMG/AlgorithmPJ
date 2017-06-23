#include <iostream>
#include <cstdio>
#include <queue>
#include <string>
#include <stack>
#include <ctime>

using namespace std;

//MAXN: node num; MAXLEN: len(a);

clock_t starttime, finishtime;

const int MAXN = 10010;
const int INF = 1000100;
const int MAXLEN = 10010;
const int MAXK = 35;

struct node
{
	int v;
	node *next;
}*e[MAXN];



string A;
int N, K;
string str[MAXN];
int f[MAXN][MAXLEN];
int state[MAXN][MAXLEN], pre[MAXN][MAXLEN];

queue<int> Q[MAXLEN];
bool inq[MAXLEN][MAXN];
char ch[MAXN];
int edit[MAXK][MAXK];

int tf[MAXLEN][MAXN], pedit[MAXLEN][MAXN];
stack<string> tS;




int min(int a, int b){
	return a<b?a:b;
}
void init() {

	cout<<"Loading Data"<<endl;
	starttime = clock();

	cin >> A;
	cin >> N;
	for (int i = 0; i < N; i++)
		cin>>str[i];
	K = str[0].length();

	for (int i = 0; i < N; i++)
		ch[i]= str[i][K - 1];

	for (int i = 0; i < N; i++)
		e[i]=NULL;

	finishtime = clock();
	cout<<"Data loaded, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;

}
void link(int u, int v) {
	//substr: pos + len
	//cout<<str[u].substr(1, K - 1)<<", "<<str[v].substr(0, K - 1)<<endl;

	if (str[u].substr(1, K - 1) != str[v].substr(0, K - 1)) return;
	//cout<<u<<","<<v<<endl;
	//
	//cout<<"Edge:"<<u<<","<<v<<endl;
	node *p;
	p = new node;
	p->v = v;
	p->next = e[u];
	e[u] = p;
}
void createGraph() {
	cout<<"Creating graph"<<endl;
	starttime = clock();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			link(i, j);

	finishtime = clock();
	cout<<"graph created, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;
}


int get_edit_distance(int nodenum) {
	for (int i = 0; i < K; i++){
		edit[i][0] = i;
		edit[0][i] = i;
	}
	for (int i = 1; i <= K; i++)
		for (int j = 1; j <= K; j++)
		{
			edit[i][j] = edit[i - 1][j] + 1;
			edit[i][j] = min(edit[i][j], edit[i][j - 1] + 1);
			if (str[nodenum][i - 1] == A[j - 1])
				edit[i][j] = min(edit[i][j], edit[i - 1][j - 1]);
			else
				edit[i][j] = min(edit[i][j], edit[i - 1][j - 1] + 1);
		}

	for (int i = 0; i <= K; i++) {
		f[nodenum][i] = edit[K][i];
		pre[nodenum][i] = -1;
		state[nodenum][i] = -1;
	}
		//cout<<"edit-dis:("<<nodenum<<"):"<<edit[K][K]<<endl;
		//return edit[K][K];
}

void pre_process() {
	//for each node i ,calc edit distance with A
	//j: A length
	cout<<"Pre_process: calculating distance f[i][K]"<<endl;
	starttime = clock();

	// set all elements to INF

	memset(f,0x3f,sizeof(f));
	for (int nodenum = 0; nodenum < N; nodenum++) {
		get_edit_distance(nodenum);
	}


	finishtime = clock();
	cout<<"Pre_process finished, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;

}


void print_ops(string A, string B)
{

	//cout<<"Printing operations"<<endl;
	//starttime = clock();

	int la, lb;
	la = A.length();
	lb = B.length();

	//cout<<la<<endl;
	//cout<<lb<<endl;
	for (int i = 0; i <= la; i++)
		tf[i][0] = i;
	for (int j = 0; j <= lb; j++)
		tf[0][j]=j;

	/*
		pedit[i][j] description;
		0: from pedit[i - 1][j]
		1: from pedit[i][j - 1]
		2: from pedit[i - 1][j - 1] (a[i] == b[j])
		3: from pedit[i - 1][j - 1] (a[i] != b[j])
	 */

	for (int i = 1; i <= la; i++)
		for (int j = 1; j <= lb; j++)
		{
			pedit[i][j] = 0;
			tf[i][j] = tf[i - 1][j] + 1;


			if (tf[i][j - 1] + 1 < tf[i][j])
			{
				tf[i][j] = tf[i][j - 1] + 1;
				pedit[i][j] = 1;
			}

			if (A[i - 1] == B[j - 1])
			{
				if (tf[i - 1][j - 1] < tf[i][j])
				{
					tf[i][j] = tf[i - 1][j - 1];
					pedit[i][j] = 2;
				}
			}
			else
			{
				if (tf[i - 1][j - 1] + 1 < tf[i][j])
				{
					tf[i][j] = tf[i - 1][j - 1] + 1;
					pedit[i][j] = 3;
				}
			}
		}



		//cout<<tf[la][lb]<<endl;
		int i, j;
		i = la; j = lb;

	/*
		pedit[i][j] description;
		0: from pedit[i - 1][j] delete i
		1: from pedit[i][j - 1] increas i
		2: from pedit[i - 1][j - 1] (a[i] == b[j])  //no operation
		3: from pedit[i - 1][j - 1] (a[i] != b[j])  // replace i with j
	 */

	//tS.clear();

		while (i > 0 || j > 0) {

			string str = "";

			if (pedit[i][j] == 0) {
			//	str  = "DEL " + to_string((j));
				str = "DEL " + to_string(i - 1);
				tS.push(str);
				i--;
				continue;
			}
			if (pedit[i][j] == 1) {
			//	tS.push("INS " + to_string(j - 1)  + " " + B[j - 1]);
				tS.push("INS " + to_string(i)  + " " + B[j - 1]);
				j--;
			}
			if (pedit[i][j] == 2) {
				i--;
				j--;
			}
			if (pedit[i][j] == 3) {
			//	tS.push("SUB " + to_string((j - 1))+ " " + B[j - 1]);
				tS.push("SUB " + to_string((i - 1))+ " " + B[j - 1]);
				i--;
				j--;
			}
		}

		while (i > 0)
		{
			tS.push("DEL " + to_string(i - 1));
			//cout<<"Rest"<<endl;
			i--;
		}
		while (j > 0)
		{
			//tS.push("INS " + to_string(j - 1) + " " + B[j - 1]);
			tS.push("INS " + to_string(0) + " " + B[j - 1]);
			j--;
		}



	while (!tS.empty()) {
		cout<<tS.top()<<endl;
		tS.pop();
	}

	//finishtime = clock();
	//cout<<"Operations printed, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;


}

void print_result() {

	int mindistance = INF;
	int pos = -1;

		/*
			cout<<"Pre:"<<endl;
			for (int i = 0; i<N;i++)
			{
				for (int j = 0; j < A.length(); j++)
					cout<<pre[i][j]<<" ";
				cout<<endl;
			}

			cout<<"State:"<<endl;
			for (int i = 0; i<N;i++)
			{
				for (int j = 0; j < A.length(); j++)
					cout<<state[i][j]<<" ";
				cout<<endl;
			}

			cout<<"f:"<<endl;
			for (int i = 0; i<N;i++)
			{
				for (int j = 0; j < A.length(); j++)
					cout<<f[i][j]<<" ";
				cout<<endl;
			}

			*/
		mindistance = INF;
		//cout<<A.length()<<endl;
		for (int i = 0; i < N; i++)
			if (mindistance > f[i][A.length() - 1]) {
				mindistance = f[i][A.length() - 1];
				pos = i;
				//cout<<i<<"mini"<<" "<<endl;
			}


		int i,j;
		i = pos; j = A.length() - 1;
		string path_str,state_str, op_str;
		path_str = "";
		state_str = "";
		op_str="";
		while (pre[i][j] >=0)
		{
			state_str  = char(48 + state[i][j]) + state_str;
			if (state[i][j] != 4) path_str = ch[i] + path_str;

			if (state[i][j] == 1)
			{
				op_str = "INS "+ to_string(j) + " " + ch[i] + "\n" + op_str;
				i = pre[i][j];
				continue;
			}
			if (state[i][j] == 2 || state[i][j] == 3)
			{
				if (state[i][j] == 3)
					op_str = "SUB "+ to_string(j) + " " + ch[i] + "\n" + op_str;
				i = pre[i][j];
				j = j - 1;
				continue;
			}
			if (state[i][j] == 4)
			{
				op_str = "DEL "+ to_string(j + 1) + "\n" + op_str;
				j = j - 1;
				continue;
			}
		}

		path_str = str[i] + path_str;

		//cout<< path_str <<endl;
		//cout<< state_str <<endl;
		//cout<< op_str <<endl;
		//cout<< "dist:"<< mindistance<<endl;
		cout<<path_str<<endl;
		cout<<mindistance<<endl;

		print_ops(A, path_str);

		//TODO: print the operations(use task1 method)
}

void work() {

	cout<<"Begin working"<<endl;
	starttime = clock();
	//i : node i; j: a[j]
	memset(inq, 0, sizeof(inq));
	//f[i][0] = K;
	//pre[i][0] = -1;
	//state[i][0] = -1;

	//for each node i ,calc edit distance with A
	//

	pre_process();
	for (int i = 0; i < N; i++){
		Q[K].push(i);
		inq[0][i] = true;
	}



	for (int j = 0; j < A.length(); j++) {

		//cout<< j <<endl;
		//cout<<"AC!"<<endl;
		int i;
		while (!Q[j].empty())
		{
			i = Q[j].front();
			//考虑用f[i][j]来更新后面的f。i为结点编号，j为在A中位置
			node *p = e[i];
			//cout<<j<<","<<i<<endl;

			while (p!= NULL)
			{
				int k=p->v;
				//cout<<j<<","<<i<<","<<k<<endl;

				if (f[k][j] >= f[i][j] + 1)
				{
					f[k][j] = f[i][j] + 1;
					pre[k][j] = i;
					state[k][j] = 1;

					if (!inq[j][k])
					{
						inq[j][k] = true;
						Q[j].push(k);
					}
				}

				if (ch[k] != A[j])
				{
					if (f[k][j + 1] >= f[i][j] + 1)
					{
						f[k][j + 1] = f[i][j] + 1;
						pre[k][j + 1] = i;
						state[k][j + 1] = 2;

						if (!inq[j+1][k])
						{
							inq[j+1][k] = true;
							Q[j+1].push(k);
						}
					}
				}
				else
				{
					if (f[k][j + 1] >= f[i][j])
					{
						f[k][j + 1] = f[i][j];
						pre[k][j + 1] = i;
						state[k][j + 1] = 3;

						if (!inq[j+1][k])
						{
							inq[j+1][k] = true;
							Q[j+1].push(k);
						}
					}
				}

				if (f[i][j + 1] >= f[i][j] + 1)
				{
					f[i][j + 1] = f[i][j] + 1;
					pre[i][j + 1] = i;
					state[i][j + 1] = 4;

					if (!inq[j+1][i])
					{
						inq[j+1][i] = true;
						Q[j+1].push(i);
					}
				}

				p=p->next;
			}
			Q[j].pop();
			inq[j][i] = false;
		}
	}

	print_result();


	finishtime = clock();
	cout<<"Work finished, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;
}

int main() {
	freopen("task2.in", "r", stdin);
	freopen("t2.out", "w", stdout);

	init();
	createGraph();
	work();
	return 0;
}

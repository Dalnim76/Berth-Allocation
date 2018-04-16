#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<time.h>
#include<map>
#include<algorithm>
using namespace std;
#define SIZE 100
#define GROUP_SIZE 100  // ��Ⱥ��ģ
#define INF 999999
#define maxInter 100  // �ܵ�������
#define PMUTATION 0.75  //ͻ����
#define craneSize 10 //��������
#define maxLine 1300//���߳������ֵ
//#define maxCrane 5 //������ʹ�õ����������
#define X1 1300 //δ��ɹ������ĺķѴ���
int asalary[4];
int esalary[4];

int T, L, N;//�ۿڷ���ʱ�����ۿڳ��ȣ�ͣ����������

int best_pos, best_f;


typedef struct Boat{
	int arritime;//������ʱ��
	int leavetime;//���涨�뿪�Ľ�ֹʱ��
	int workload;//������
	int boatwidth;//�����
	int goodlocation;//ƫ�õĴ�ͣ��λ��
	int bertlocation;//��ͣ��λ��
	int starttime;//ʵ��ͣ����ʱ��
	int endtime;//ʵ���뿪��ʱ��
	int ID;
	bool find;
	int boatMaxCrane;//������ֻʹ�õ����������
	vector<int> percrane;//ж��ʱ��ÿСʱ��������
	Boat() {

	}

} Boat;
void cpBoat(Boat src[], Boat dst[]);
typedef struct Group{
	int order[SIZE];
	Boat boat[SIZE];
	Group() {

	}
	int f;
	bool operator <(const Group& s) {
		if (f < s.f) {
			return true;
		}
		else
		{
			return false;
		}
	}
	Group(Group& s) {
		cpBoat(s.boat, boat);
		for (int i = 0; i < SIZE; i++) {
			order[i] = s.order[i];
		}
		f = s.f;
	}
}Group;
int cmp(const Group& a, const Group& b) {
	if (a.f > b.f)
		return 1;
	else
	{
		return 0;
	}
}
typedef struct Record {
	int start;
	int end;
	int bertlocation;
	vector<int> percrane;
	bool find;
	int cost;
	Record() :find(false),cost(0) {

	}
	Record(Record& s) {
		this->start = s.start;
		this->bertlocation = s.bertlocation;
		this->cost = s.cost;
		this->end = s.end;
		this->find = s.find;
		this->percrane.clear();
		for (int i = 0; i < s.percrane.size(); i++) {
			this->percrane.push_back(s.percrane[i]);
		}
	}
}Record;



void cpBoat(Boat src[], Boat dst[]) {
	for (int i = 0; i < N; i++) {
		dst[i].arritime = src[i].arritime;
		dst[i].leavetime = src[i].leavetime;
		dst[i].workload = src[i].workload;
		dst[i].boatwidth = src[i].boatwidth;
		dst[i].bertlocation = src[i].bertlocation;
		dst[i].starttime = src[i].starttime;
		dst[i].endtime = src[i].endtime;
		dst[i].goodlocation = src[i].goodlocation;
		dst[i].ID = src[i].ID;
		dst[i].find = src[i].find;
		dst[i].boatMaxCrane = src[i].boatMaxCrane;
		dst[i].percrane.clear();
		for (int j = 0; j < src[i].percrane.size(); j++) {
			dst[i].percrane.push_back(src[i].percrane[j]);
		}

	}
}

void cpOneBoat(Boat& src, Boat& dst) {
	dst.arritime = src.arritime;
	dst.leavetime = src.leavetime;
	dst.workload = src.workload;
	dst.boatwidth = src.boatwidth;
	dst.bertlocation = src.bertlocation;
	dst.starttime = src.starttime;
	dst.endtime = src.endtime;
	dst.goodlocation = src.goodlocation;
	dst.ID = src.ID;
	dst.find = src.find;
	dst.boatMaxCrane = src.boatMaxCrane;
	dst.percrane.clear();
	for (int j = 0; j < src.percrane.size(); j++) {
		dst.percrane.push_back(src.percrane[j]);
	}
}

void swapBoat(Boat& a, Boat& b) {
	Boat tmp;
	cpOneBoat(a, tmp);
	cpOneBoat(b, a);
	cpOneBoat(tmp, b);
}

void swapGroup(Group& a, Group& b) {
	for (int i = 0; i < N; i++) {
		swap(a.order[i], b.order[i]);
		swapBoat(a.boat[i], b.boat[i]);
	}
	swap(a.f, b.f);
	
}
int mp_crane[SIZE];//mp_crane[t]��ʾtʱ��ʱ��ʣ����а�����
bool mp_bertlocation[SIZE][maxLine];//ͣ��λ�ú�ʱ��Ķ�άռ��ͼ
Boat a[SIZE];
Boat a_copy[SIZE];
Group g[GROUP_SIZE];
Group children;
void init() {
	//memset(mp_crane, false, sizeof(mp_crane));
	memset(mp_bertlocation, 0, sizeof(mp_bertlocation));
	for (int i = 0; i < SIZE; i++) {
		mp_crane[i] = craneSize;
	}
	//f = INF;
	//min_f = INF;
}

bool _in(int x, int y) {
	return (x < T && y < L);
}
void fill(int x1, int x2, int y1, int y2) {
	for (int i = x1; i < x2; i++) {
		for (int j = y1; j < y2; j++) {
			mp_bertlocation[i][j] = true;
		}
	}
}
void random_N(int a[]) {
	for (int i = N - 1; i > 0; i--) {
		int index = rand() % N;
		swap(a[i], a[index]);
	}
}
bool isEmpty(Boat& a, int t, int l) {
	for (int i = l; i < l + a.boatwidth; i++)
	{
		if (mp_bertlocation[t][i] == true)
			return false;
	}
	return true;
}
Record endTime(Boat& a, int t, int l) {
	int load = 0;
	int end = t;
	Record re;
	re.bertlocation = l;
	int maxCrane = a.boatMaxCrane;
	
	while (true)
	{
		if (!isEmpty(a, end, l)) {
			break;
		}
		if (mp_crane[end] >= maxCrane) {
			int tmpload = maxCrane * 30;
			if (tmpload >= (a.workload-load))
			{	
				re.percrane.push_back(ceil(((double)(a.workload-load))/30.0));
				load = a.workload;
				re.find = true;
				end++;
				break;				
			}
			else
			{
				load += tmpload;
				re.percrane.push_back(maxCrane);

			}
		}
		else
		{
			int tmpload = mp_crane[end] * 30;
			if (tmpload >= (a.workload - load))
			{				
				re.percrane.push_back(ceil(((double)(a.workload - load)) / 30.0));
				load = a.workload;
				re.find = true;
				end++;
				break;
			}
			else
			{
				load += tmpload;
				re.percrane.push_back(mp_crane[end]);

			}
		}
		end++;
		if (end > T) {
			break;
		}
	}
	if (re.find) {
		re.start = t;
		re.end = end;
		re.cost += (end - a.leavetime)*20;
		re.cost += abs(l - a.goodlocation)*3;
		for (int i = t; i < end; i++) {
			int s = i % 24;
			if (s >= 0 && s <= 5) {
				re.cost += re.percrane[i - t] * asalary[0];
				//re.cost += re.percrane[i - t] * esalary[0];
			}
			else if (s>=6 && s<=11) {
				re.cost += re.percrane[i - t] * asalary[1];
				//re.cost += re.percrane[i - t] * esalary[1];
			}
			else if (s>=12 && s<=17) {
				re.cost += re.percrane[i - t] * asalary[2];
				//re.cost += re.percrane[i - t] * esalary[2];
			}
			else
			{
				re.cost += re.percrane[i - t] * asalary[3];
				//re.cost += re.percrane[i - t] * esalary[3];
			}
		}
	}
	else
	{
		re.cost = INF;
	}
	return re;
}

int Greedy(Boat a[])//̰�İڷ�
{
	init();
	int cost = 0;
	for (int i = 0; i < N; i++) {
		bool find = false;
		Record re;
		for (int t = a[i].arritime; t < T; t++) {
			int maxl = max((L - a[i].boatwidth - a[i].goodlocation), a[i].bertlocation);
			if (mp_crane[t] == 0)
				continue;
			for (int l = 0; l < maxl; l++) {
				int l1 = a[i].goodlocation + l;
				int l2 = a[i].goodlocation - l;
				if (l1 < L - a[i].boatwidth) {
					Record tmp = endTime(a[i], t, l1);
					if (tmp.find) {
						//cout << "boat ID:" << a[i].ID << " ";
						//cout << "cost" << tmp.cost << endl;
						if (!re.find)
							re = tmp;
						else
						{
							if (re.cost > tmp.cost) {
								re = tmp;
							}
						}
						
					}
				}
				if (l2 >= 0) {
					Record tmp = endTime(a[i], t, l1);
					if (tmp.find) {
						//cout << "boat ID:" << a[i].ID << " ";
						//cout << "cost" << tmp.cost << endl;
						if (!re.find)
							re = tmp;
						else
						{
							if (re.cost > tmp.cost) {
								re = tmp;
							}
						}
						
					}
				}
				if (re.find)
					break;
			}
			if (re.find) {
				break;
			}
		}
		if (re.find)
		{
			cost += re.cost;
			a[i].bertlocation = re.bertlocation;
			a[i].endtime = re.end;
			a[i].starttime = re.start;
			a[i].percrane = re.percrane;
			//cout << "boat ID:" << a[i].ID << ";" <<"boat width:"<<a[i].boatwidth<<endl;
			for (int j = re.start; j < re.end; j++) {
				//cout << "ʱ�䣺" << j << " ";
				for (int x = re.bertlocation; x < (re.bertlocation + a[i].boatwidth); x++) {
					mp_bertlocation[j][x] = true;
				}
				mp_crane[j] -= re.percrane[j - re.start];
				//cout << "usedcranesize:" << re.percrane[j - re.start] << endl;
			}
			a[i].find = true;
			//cout << endl;
		}			
		else
		{
			a[i].find = false;
			cost += X1;
			//cout << "boat ID:" << a[i].ID << "û�к��ʵ�λ��" << endl;
		}
	}
	return cost;
}

//����
void cross(int start_pos, int end_pos, int parent1[], int parent2[]) {
	bool used[SIZE];
	memset(used, false, sizeof(used));
	for (int i = 0; i < N; i++) {
		children.order[i] = 0;
	}
	if (start_pos > end_pos) {
		swap(start_pos, end_pos);
	}
	for (int i = start_pos; i <= end_pos; i++) {
		children.order[i] = parent1[i];
		used[parent1[i]] = true;
	}
	for (int i = 0; i < N; i++) {
		if (i >= start_pos&&i <= end_pos)
			continue;
		for (int j = 0; j < N; j++) {
			if (!used[parent2[j]]) {
				children.order[i] = parent2[j];
				used[parent2[j]] = true;
				break;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		cpOneBoat(a[children.order[i]], children.boat[i]);
	}

}

FILE *in, *out;

int main() {
	init();
	int tmp_salary[] = { 70,50,50,70 };
	for (int i = 0; i < 4; i++) {
		asalary[i] = tmp_salary[i];
		esalary[i] = tmp_salary[i];
	}
	//�ۿڷ�����ʱ���������������ۿ��ܳ���
	T = 168;
	N = 15;
	L = 1200;
	int seed = (unsigned)time(NULL);
	srand(seed);

	
	int ret=fopen_s(&in,"in.txt", "r");
	if (ret!=0) {
		cout << "no find in.txt" << endl;
		return 0;
	}
	for (int i = 0; i < N; i++) {
		a[i].ID = i;
		//���봬����Ϣ������ʱ�䣬����ƫ�õ�ͣ��λ�ã������뿪ʱ�䣬����ж��������,�����ҵ����������
		
		//cin >> a[i].arritime >> a[i].goodlocation >> a[i].workload >> a[i].boatwidth >>  a[i].leavetime >> a[i].boatMaxCrane;
		fscanf_s(in, "%d %d %d %d %d %d\n", &(a[i].arritime), &(a[i].goodlocation), &(a[i].workload), &(a[i].boatwidth), &(a[i].leavetime), &(a[i].boatMaxCrane));
		
	}

	//��ʼ����Ⱥ
	for (int i = 0; i < GROUP_SIZE; i++) {
		cpBoat(a, a_copy);
		int a_tmp[SIZE] = { 0, };
		for (int j = 0; j < N; j++) {
			a_tmp[j] = a_copy[j].ID;
		}
		/*cout << "ԭʼ����";
		for (int j = 0; j < N; j++) {
			 cout<< a_tmp[j] << " ";
		}
		cout << endl;*/
		random_N(a_tmp);
		/*cout << "�������";
		for (int j = 0; j < N; j++) {
			cout<< a_tmp[j] << " ";
		}
		cout << endl;*/
		for (int j = 0; j < N; j++) {
			g[i].order[j] = a_tmp[j];
			cpOneBoat(a_copy[a_tmp[j]],g[i].boat[j]);			
		}
		g[i].f=Greedy(g[i].boat);
		//cout <<"��i����Ⱥ "<<i<<": cost"<< g[i].f << endl;
		//cout << "********************************" << endl;
	}

	//���̷�����ʣ����ȼ�����Ⱥ��Ӧ�ȣ�������Ӧ�ȣ���ÿ������������̸��ʣ�
	for (int ti = 1; ti <= maxInter; ti++) 
	{
		int sum_f = 0;
		int fitness[GROUP_SIZE] = { 0, };//��Ⱥ������Ӧ��ֵ
		double p[GROUP_SIZE] = { 0.0, };//��Ⱥ������Ӧ�ȸ���
		map<double, int>pie;//ÿ��������ۻ�������ɵ���ת����
		pie.clear();
		for (int i = 0; i < GROUP_SIZE; i++) {
			sum_f += g[i].f;
		}
		for (int i = 0; i < GROUP_SIZE; i++) {
			fitness[i] = sum_f - g[i].f;//��Ϊ��ǰ�����g[i]��fԽ��֤���˷���Խ���ã�Ӧ�ø���ĸ���ԽС
			p[i] = 1.0*fitness[i] / sum_f;
		}
		double ret = 0.0;
		for (int i = 0; i < GROUP_SIZE; i++) {
			ret += p[i];
			pie[ret] = i;
		}
		int number1, number2;
		do
		{
			double p1 = (double)rand() / RAND_MAX;
			double p2 = (double)rand() / RAND_MAX;
			number1 = pie.lower_bound(p1)->second;//�ҳ����ʱȽϴ�ĸ���
			number2 = pie.lower_bound(p2)->second;//�ҳ����ʱȽϴ�ĸ���
		} while (number1==number2);

		int number3 = rand() % N;
		int number4 = rand() % N;
		cross(number3, number4, g[number1].order, g[number2].order);//��������Ӵ���
		
		if (PMUTATION > ((double)rand() / RAND_MAX)) {
			for (int k = 1; k <= 4; k++) {
				int i1 = rand() % N;
				int i2 = rand() % N;
				swapBoat(children.boat[i1], children.boat[i2]);
				swap(children.order[i1], children.order[i2]);
			}
		}
		children.f = Greedy(children.boat);

		//����Ӵ��������Ⱥ��ѡ����Ⱥ�б������ĸ��壬���н�����
		int worst_f = 0;
		int worst_pos = -1;
		best_f = INT_MAX;
		best_pos = -1;
		for (int i = 0; i < GROUP_SIZE; i++) {//Ѱ���ڲ���Ӧ��������
			//cout << i << " " << g[i].f << endl;
			if (g[i].f > worst_f) {
				worst_f = g[i].f;
				worst_pos = i;
			}
			if (g[i].f < best_f) {
				best_f = g[i].f;
				best_pos = i;
			}
		}
		if (children.f < worst_f) {
			swapGroup(children, g[worst_pos]);
		}
		sort(g, g + GROUP_SIZE);
		int meanf = 0;
		for (int i = 0; i < 10; i++) {
			meanf += g[i].f;
		}
		meanf = meanf / 10;
		cout << "����������" << ti << "��Ӧֵ" <<meanf << endl;
		//cout << "����������" << ti << "��Ӧֵ" << g[0].f << endl;
	}
	best_pos = 0;
	for (int i = 0; i < N; i++) {
		Boat* s=&(g[best_pos].boat[i]);
		cout << "boat ID:" << s->ID << ";";
		if (!s->find) {
			cout << "no find" << endl;
			continue;
		}
		cout<< "boat width:" << s->boatwidth << ";stoplocation" << s->bertlocation<< endl;
		cout << "starttime:" << s->starttime << ";endtime:" << s->endtime << endl;
		cout << "usedcranesize:";
		for (int j = s->starttime; j < s->endtime; j++) {		
			cout << s->percrane[j - s->starttime] <<" "<< endl;
		}
		cout << "****************"<<endl;
	}
	cout << best_f << endl;






}
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

void read_solution(vector<vector<int>>& data)
{
	ifstream infile("./output/solution.txt");

	string content;
	while (getline(infile, content))
	{
		vector<int> row_int;
		while (content.find('<') != -1)
		{
			int find1 = content.find('<');
			int find2 = content.find('>', find1);
			string temp = content.substr(find1, find2 - find1 + 1);
			int dou_ = temp.find(',');
			string nums = temp.substr(dou_ + 1, temp.size() - dou_ - 2);
			int num = stoi(nums);

			row_int.push_back(num);
			if (content.size() >= find2 + 2)
				content = content.substr(find2 + 2, content.size() - find2 + 1);
			else
				break;
		}
		data.push_back(row_int);
	}
}

//读取site_bandwidth.csv数据
bool read_bandwidth(string& filename, vector<int>& data)
{
	ifstream infile("/data/site_bandwidth.csv");      
	if (!infile.is_open())
		return false;

	string content;	
	getline(infile, content);

	while (getline(infile, content))
	{
		int dou = content.find(',');
		string nums = content.substr(dou + 1, content.size() - dou - 1);
		data.push_back(stoi(nums));
	}
	return true;
}


bool read_qos(string& filename, vector<vector<int>>& data, vector<string>& cuntomer_id,
	vector<string>& site_name)
{
	ifstream infile("/data/qos.csv");
	if (!infile.is_open())
		return false;

	string content;
	getline(infile, content);
	content.pop_back();
	int first_dou = content.find(',');
	content = content.substr(first_dou + 1, content.size() - first_dou - 1);
	while (content != "")
	{
		int dou = content.find(',');
		if (dou == -1)         
		{
			cuntomer_id.push_back(content);
			break;
		}
		string name = content.substr(0, dou);
		cuntomer_id.push_back(name);
		content = content.substr(dou + 1, content.size() - dou - 1);
	}

	while (getline(infile, content))
	{
		int dou = content.find(',');
		string name = content.substr(0, dou);
		site_name.push_back(name);
		content = content.substr(dou + 1, content.size() - dou - 1);

		vector<int> single_row;
		while ((dou = content.find(',')) != -1)
		{
			string nums = content.substr(0, dou);
			single_row.push_back(stoi(nums));
			content = content.substr(dou + 1, content.size() - 1 - dou);
		}
		single_row.push_back(stoi(content));
		data.push_back(single_row);
	}
}

bool read_demand(string& filename, vector<vector<int>>& data)
{
	ifstream infile("/data/demand.csv");
	if (!infile.is_open())
		return false;
	string content;
	getline(infile, content);

	while (getline(infile, content))
	{
		int dou = content.find(',');
		content = content.substr(dou + 1, content.size() - dou - 1);

		vector<int> single_row;
		while ((dou = content.find(',')) != -1)
		{
			string nums = content.substr(0, dou);
			single_row.push_back(stoi(nums));
			content = content.substr(dou + 1, content.size() - dou - 1);
		}
		single_row.push_back(stoi(content));
		data.push_back(single_row);
	}
}

bool read_config(string filename, int& constraint_qos)
{
	ifstream infile("/data/config.ini");
	if (!infile.is_open())
		return false;

	string content;
	getline(infile, content);
	getline(infile, content);

	int den = content.find('=');
	string nums = content.substr(den + 1, content.size() - den);
	constraint_qos = stoi(nums);

	return true;
}


bool read_source(vector<string>& DIR_SRCS, vector<vector<int>>& demand,
	vector<vector<int>>& qos, vector<int>& site_bandwidth, vector<string>& customer_id,
	vector<string>& site_name, int& constraint_qos)
{
	bool isread0 = read_demand(DIR_SRCS[0], demand);
	bool isread1 = read_qos(DIR_SRCS[1], qos, customer_id, site_name);
	bool isread2 = read_bandwidth(DIR_SRCS[2], site_bandwidth);
	bool isread3 = read_config(DIR_SRCS[3], constraint_qos);

	if (isread0 && isread1 && isread2 && isread3)
		return true;
}

bool cmp1(pair<int, int>& j1, pair<int, int>& j2)
{
	return j1.second > j2.second;
}

bool cmp2(pair<int, pair<int, int>>& j1, pair<int, pair<int, int>>& j2)
{
	if (j1.second.first == j2.second.first)
		return j1.second.second < j2.second.second;
	return j1.second.first > j2.second.first;
}

bool cmp3(pair<int, pair<int, int>>& j1, pair<int, pair<int, int>>j2)
{
	if (j1.second.first == j2.second.first)
		return j1.second.second > j2.second.second;
	return j1.second.first > j2.second.first;
}

vector<int> bubbleSort(vector<int>& list)  
{
	vector<int>index;
	for (int i = 0; i < list.size(); i++)
		index.push_back(i);

	if (list.size() < 2)
	{
		return index;
	}

	for (int end = list.size() - 1; end > 0; end--)
	{
		for (int i = 0; i < end; i++)
		{
			if (list[i] < list[i + 1])
			{
				swap(list[i], list[i + 1]);
				swap(index[i], index[i + 1]);
			}
		}
	}
	return index;
}

void bubbleSort2(vector<int>& list, vector<int>& index)  
{
	if (list.size() < 2)
	{
		return;
	}

	for (int end = list.size() - 1; end > 0; end--)
	{
		for (int i = 0; i < end; i++)
		{
			if (list[i] > list[i + 1])
			{
				swap(list[i], list[i + 1]);
				swap(index[i], index[i + 1]);
			}
		}
	}
}

void decide(vector<vector<int>>& demand, vector<vector<int>>& qos, vector<int>& site_bandwidth,
	int& constraint_qos, vector<vector<pair<int, int>>>& result,
	vector<string>& customer_id, vector<string>& site_name)
{
	int max_percent5 = demand.size() * 0.05;
	vector<vector<int>>orig_demand = demand;

	//remain_band[i][j]表示第i个时刻，第j个边缘节点剩余多少带宽,
	vector<vector<int>> remain_band;
	for (int i = 0; i < demand.size(); i++)
		remain_band.push_back(site_bandwidth);

	vector<vector<int>> could_offer(site_bandwidth.size());       //表示第i个边缘节点可以给第j个客户点分配
	for (int i = 0; i < qos.size(); i++)
	{
		for (int j = 0; j < qos[i].size(); j++)
		{
			if (qos[i][j] < constraint_qos)
				could_offer[i].push_back(j);
		}
	}
	//将所有edge按照，先带宽值大，后覆盖节点多的顺序排序
	vector<pair<int, pair<int, int>>> band_with_offer;
	for (int i = 0; i < site_bandwidth.size(); i++)
	{
		pair<int, int> sec = make_pair(site_bandwidth[i], could_offer[i].size());
		band_with_offer.push_back(make_pair(i, sec));
	}
	sort(band_with_offer.begin(), band_with_offer.end(), cmp3);

	//vector<int> sort_index_band = bubbleSort(site_bandwidth);


	for (int k = 0; k < band_with_offer.size(); k++)
	{		
		vector<pair<int, int>> single_node_deal_row;
		int index_edge = band_with_offer[k].first;
		for (int i = 0; i < demand.size(); i++)
		{
			int sumNum = 0;
			int band = site_bandwidth[k];
			vector<int> demand_row = demand[i];
			for (int j = 0; j < could_offer[index_edge].size(); j++)
			{
				int col = could_offer[index_edge][j];
				if (band >= demand_row[col])
				{
					sumNum += demand_row[col];
					band -= demand_row[col];
				}
				else
				{
					sumNum += band;
					demand_row[col] -= band;
				}
			}
			single_node_deal_row.push_back(make_pair(i, sumNum));
		}
		sort(single_node_deal_row.begin(), single_node_deal_row.end(), cmp1);
		for (int m = 0; m < max_percent5; m++)
		{
			int rowNum = single_node_deal_row[m].first;
			for (int n = 0; n < could_offer[index_edge].size(); n++)
			{
				int colNum = could_offer[index_edge][n];
				if (demand[rowNum][colNum] == 0)
					continue;
				if (remain_band[rowNum][index_edge] >= demand[rowNum][colNum])
				{
					result[rowNum * customer_id.size() + colNum].push_back(
						make_pair(index_edge, demand[rowNum][colNum]));
					remain_band[rowNum][index_edge] -= demand[rowNum][colNum];
					demand[rowNum][colNum] = 0;
				}
				else
				{
					if (remain_band[rowNum][index_edge] != 0)
					{
						result[rowNum * customer_id.size() + colNum].push_back(
							make_pair(index_edge, remain_band[rowNum][index_edge]));
						demand[rowNum][colNum] -= remain_band[rowNum][index_edge];
						remain_band[rowNum][index_edge] = 0;
					}
				}
			}
		}
	}

	for (int i = 0; i < demand.size(); i++)
	{
		for (int j = 0; j < demand[i].size(); j++)
		{
			if (demand[i][j] != 0)
			{
				vector<int> record_fillj_index;
				for (int k = 0; k < remain_band[i].size(); k++)
				{
					if (qos[k][j] < constraint_qos && remain_band[i][k] > 0)
						record_fillj_index.push_back(k);
				}
				int last = 0;

				int remain = demand[i][j] % record_fillj_index.size();
				int avg = demand[i][j] / record_fillj_index.size();
				demand[i][j] = 0;

				vector<int> temp;
				vector<int> index_temp;
				for (int m = 0; m < record_fillj_index.size(); m++)
				{
					temp.push_back(remain_band[i][record_fillj_index[m]]);
					index_temp.push_back(record_fillj_index[m]);
				}

				bubbleSort2(temp, index_temp);
				int reduce_remain = 0;
				for (int m = 0; m < remain; m++)
				{
					if (temp[m] < avg + 1)
					{
						result[i * customer_id.size() + j].push_back(
							make_pair(index_temp[m], temp[m]));
						reduce_remain = avg + 1 - temp[m];
						temp[m] = 0;
						remain_band[i][index_temp[m]] = 0;
					}
					else
					{
						if (temp[m] >= avg + 1 + reduce_remain)
						{
							result[i * customer_id.size() + j].push_back(
								make_pair(index_temp[m], avg + 1 + reduce_remain));
							temp[m] -= avg + 1 + reduce_remain;
							remain_band[i][index_temp[m]] -= avg + 1 + reduce_remain;
							reduce_remain = 0;
						}
						else
						{
							result[i * customer_id.size() + j].push_back(
								make_pair(index_temp[m], temp[m]));
							reduce_remain = avg + 1 + reduce_remain - temp[m];
							temp[m] = 0;
							remain_band[i][index_temp[m]] = 0;
						}
					}
				}
				for (int m = remain; m < temp.size(); m++)
				{
					if (temp[m] < avg)
					{
						result[i * customer_id.size() + j].push_back(
							make_pair(index_temp[m], temp[m]));
						reduce_remain = avg - temp[m];
						temp[m] = 0;
						remain_band[i][index_temp[m]] = 0;
					}
					else
					{
						if (temp[m] >= avg + reduce_remain)
						{
							result[i * customer_id.size() + j].push_back(
								make_pair(index_temp[m], avg + reduce_remain));
							temp[m] -= avg + reduce_remain;
							remain_band[i][index_temp[m]] -= avg + reduce_remain;
							reduce_remain = 0;
						}
						else
						{
							result[i * customer_id.size() + j].push_back(
								make_pair(index_temp[m], temp[m]));
							reduce_remain = avg + reduce_remain - temp[m];
							temp[m] = 0;
							remain_band[i][index_temp[m]] = 0;
						}
					}
				}
			}
		}
	}

	ofstream outfile("/output/solution.txt");
	for (int i = 0; i < result.size(); i++)
	{
		outfile << customer_id[i % customer_id.size()] << ':';
		if (result[i][0].first == -1)
		{
			outfile << endl;
			continue;
		}
		for (int j = 0; j < result[i].size() - 1; j++)
		{
			outfile << '<';
			outfile << site_name[result[i][j].first] << ',';
			outfile << result[i][j].second << '>';
			outfile << ',';
		}
		//因为最后一个不需要逗号，所以最后单独处理

		outfile << '<';
		outfile << site_name[result[i].back().first] << ',';
		outfile << result[i].back().second << '>';
		outfile << endl;
	}
	outfile.close();
}

int main()
{
	vector<string> DIR_SRCS = { "demad.csv","qos.csv","site_bandwidth.csv","config.ini" };
	vector<vector<int>> demand;
	vector<vector<int>> qos;
	vector<int> site_bandwidth;       //以索引代表边缘节点
	vector<string> customer_id;      //依靠索引寻找原来的名字
	vector<string> site_name;        //依靠索引寻找原来的名字
	int constraint_qos;
	if (read_source(DIR_SRCS, demand, qos, site_bandwidth, customer_id, site_name, constraint_qos))
		cout << "正确读取数据" << endl;

	//前一个代表索引为i的边缘节点，第二个int表示带宽,用result的索引代表时间
	vector<vector<pair<int, int>>> result(demand.size() * customer_id.size());
	decide(demand, qos, site_bandwidth, constraint_qos, result, customer_id, site_name);

}

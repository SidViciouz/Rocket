#include "Model.h"
#include "IfError.h"

string getToken(string& aLine, bool isFirst); 
int getNumber(string& aWord, bool isFirst);

Model::Model(ID3D12Device* device,const char* fileName, ID3D12GraphicsCommandList* commandList)
{
	ifstream ifs;
	ifs.open(fileName,ios_base::in);

	if (ifs.fail())
		IfError::Throw(L"model file open error!");

	float x, y, z;
	vector<XMFLOAT3> positions;
	vector<XMFLOAT2> uvs;
	vector<XMFLOAT3> normals;
	uint16_t j = 0;

	while (!ifs.eof())
	{
		string aLine;
		getline(ifs, aLine);

		string type = getToken(aLine, true);

		if (type.compare("v") == 0)
		{
			string value;
			x = stof(getToken(aLine, false));
			y = stof(getToken(aLine, false));
			z = stof(getToken(aLine, false));
			positions.push_back({ x,y,z });
		}
		else if (type.compare("vt") == 0)
		{
			string value;
			x = stof(getToken(aLine, false));
			y = stof(getToken(aLine, false));
			uvs.push_back({ x,y});
		}
		else if (type.compare("vn") == 0)
		{
			string value;
			x = stof(getToken(aLine, false));
			y = stof(getToken(aLine, false));
			z = stof(getToken(aLine, false));
			normals.push_back({ x,y,z });
		}
		else if (type.compare("f") == 0)
		{
			string value;
			vector<int> index;
			while ((value = getToken(aLine, false)).compare(" ") != 0)
			{
				index.push_back(getNumber(value, true)-1);
				index.push_back(getNumber(value, false)-1);
				index.push_back(getNumber(value, false)-1);
			}

			if (index.size() == 9)
			{
				for (int i = 0; i < 9; i = i + 3)
				{
					mVertices.push_back({ positions[index[i]],uvs[index[i + 1]],normals[index[i + 2]] });
					mIndices.push_back(j++);
				}
			}
			else
			{
				for (int i = 0; i < 12; i = i + 3)
					mVertices.push_back({ positions[index[i]],uvs[index[i + 1]],normals[index[i + 2]] });
				mIndices.push_back(j);
				mIndices.push_back(j+1);
				mIndices.push_back(j+2);
				mIndices.push_back(j);
				mIndices.push_back(j+2);
				mIndices.push_back(j+3);
				j = j + 4;
				
			}
		}
	}

	ifs.close();

	mVertexBufferSize = mVertices.size();
	mIndexBufferSize = mIndices.size();

	mVertexBufferOffset = mAllVertices.size();
	mIndexBufferOffset = mAllIndices.size();

	mAllVertices.insert(mAllVertices.end(), mVertices.begin(), mVertices.end());
	mAllIndices.insert(mAllIndices.end(), mIndices.begin(), mIndices.end());
}

// string에서 공백전까지의 token을 반환, 문자열의 끝이라면 " "를 반환
// 입력 문자열의 시작부터 token을 반환하려면 isFirst = true를 입력
// 토큰을 반환한 적 있고, 그 다음부터 이어서 반환하려면 isFirst = false를 입력한다.
string getToken(string& aLine,bool isFirst)
{
	static int startPosition = 0;

	if (isFirst)
		startPosition = 0;

	while (aLine.length() > startPosition &&
		(aLine[startPosition] == ' ' || aLine[startPosition] == '\n' || aLine[startPosition] == '\t'))
		++startPosition;

	if (aLine.length() <= startPosition)
		return " ";

	int endPosition = startPosition;

	while (aLine.length() > endPosition &&
		aLine[endPosition] != ' ' && aLine[endPosition] != '\n' && aLine[endPosition] != '\t')
		++endPosition;

	int length = endPosition - startPosition;
	int start = startPosition;

	startPosition = endPosition + 1;

	return aLine.substr(start,length);
}

// a/b/c와 같은 형태에서 '/'전까지 반환 끝이라면 " "를 반환
// 입력 문자열의 시작부터 token을 반환하려면 isFirst = true를 입력
// 토큰을 반환한 적 있고, 그 다음부터 이어서 반환하려면 isFirst = false를 입력한다.
int getNumber(string& aWord, bool isFirst)
{
	static int startPosition = 0;

	if (isFirst)
		startPosition = 0;

	if (aWord.length() <= startPosition || aWord[startPosition] == '/')
	{
		startPosition += 1;
		return 0;
	}

	int endPosition = startPosition;

	while (aWord.length() > endPosition && aWord[endPosition] != '/')
		++endPosition;

	string numberString = aWord.substr(startPosition, endPosition - startPosition);
	startPosition = endPosition + 1;

	return stoi(numberString);
}

unique_ptr<Buffer> Model::mVertexBuffer;
unique_ptr<Buffer> Model::mIndexBuffer;

vector<Vertex> Model::mAllVertices;
vector<uint16_t> Model::mAllIndices;

int Model::mNextObjConstantIndex = 0;
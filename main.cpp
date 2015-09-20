#include<iostream>
#include<fstream>
#include<string>
#include <queue>
#include <windows.h>
#include <strsafe.h>
#include <ctime>
#include <sstream>

using namespace std;

struct SubscriberInfo
{
	string ItemID;
	string MerchantID;
	string MarketplaceID;
	string DataType;
};



queue <SubscriberInfo*> Qlow , Qlowest , Qnormal , Qhigh , Qhighest;

void ProcessTextFiles(char *txtfile);
string DisplayCurrentTime();

int main(int argc , char *argv[])
{

	if(argc < 3)
	{
		cout<<"Syntax :"<<endl<<endl;
		cout<<"UpstreamReader DumpPath CompletedPath"<<endl<<endl;
		cout<<"DumpPath = C:\\Users\\navin\\Desktop\\Hackathon\\dump\\"<<endl;
		cout<<"CompletedPath = C:\\Users\\navin\\Desktop\\Hackathon\\complete\\"<<endl;
		return 0;
	}

	cout<<"-----> Upstream Reader process has started"<<endl<<endl;

	while(true)

	{
		cout<<"-----> Upstream Reader process is running "<<DisplayCurrentTime()<<endl<<endl;

		WIN32_FIND_DATA ffd;
		LARGE_INTEGER filesize;
		TCHAR szDir[MAX_PATH];
		HANDLE hFind ;
		DWORD dwError=0;
		hFind = INVALID_HANDLE_VALUE;
		char temp[1024],NewPath[1024];

		//string directory("C:\\Users\\navin\\Desktop\\Hackathon\\dump\\");
		string directory = (char*)argv[1];
		//strcpy(NewPath,(char*)argv[2]);


		hFind = FindFirstFile((directory + "/*.txt").c_str(), &ffd);

		if (INVALID_HANDLE_VALUE != hFind)
		{
			// List all the files in the directory with some info about them.
			cout<<"-----> Upstream Reader Process Started Processing Text File(s)  "<< DisplayCurrentTime()<<endl<<endl;


			do
			{
				strcpy(NewPath,(char*)argv[2]);

				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					//cout <<ffd.cFileName<<endl;
				}
				else
				{
					filesize.LowPart = ffd.nFileSizeLow;
					filesize.HighPart = ffd.nFileSizeHigh;
					//cout<<ffd.cFileName<<" "<< filesize.QuadPart<<endl;

					// process text files and dump to another folder

					strcpy(temp,directory.c_str());
					strcat(temp,ffd.cFileName);

					ProcessTextFiles(temp);
				}


				strcat(NewPath,ffd.cFileName);
				MoveFile(temp,NewPath);

				//strcpy(NewPath,"C:\\Users\\navin\\Desktop\\Hackathon\\complete\\");
			}
			while (FindNextFile(hFind, &ffd) != 0);

			dwError = GetLastError();

			if (dwError != ERROR_NO_MORE_FILES) 
				return -1;


			FindClose(hFind);
			cout<<"-----> Upstream Reader Process Completed Processing Text File(s)  "<< DisplayCurrentTime()<<endl<<endl;
		}

		Sleep(1000);

	}
	return 0;;

}
string DisplayCurrentTime()
{
	time_t  timev;
	time(&timev);
	std::stringstream ss;

	struct tm * now = localtime( & timev );

	ss << (now->tm_year + 1900) << '-' 
		<< (now->tm_mon + 1) << '-'
		<<  now->tm_mday << " "
		<<  now->tm_hour<< ':' 
		<<  now->tm_min<< ':' 
		<<  now->tm_sec
		<< endl;

	std::string s = ss.str();

	return s;
}
void ProcessTextFiles(char *textfile)
{

	ifstream file(textfile);
	string oneline;
	while (getline(file, oneline))
	{
		char* cstr=const_cast<char*>(oneline.c_str());
		char* current;
		int counter = -1;
		int flag = -1;

		SubscriberInfo *ptr = new SubscriberInfo();
		current=strtok(cstr,",");

		if(current != NULL)
		{
			strcpy((char*)ptr->ItemID.c_str(),current);
			counter++;
		}

		while(current!=NULL){

			current=strtok(NULL,",");

			if(current != NULL)

			{
				counter++;

				if(counter == 1)
					strcpy((char*)ptr->MerchantID.c_str(),current);
				else if(counter == 2)
					strcpy((char*)ptr->MarketplaceID.c_str(),current);
				else if(counter == 3)
				{
					if(!strcmp(current,"LOW"))
						flag = 1;
					else if(!strcmp(current,"LOWEST"))
						flag = 2;
					else if(!strcmp(current,"NORMAL"))
						flag = 3;
					else if(!strcmp(current,"HIGH"))
						flag = 4;
					else if(!strcmp(current,"HIGHEST"))
						flag = 5;

				}
				else if(counter == 4)
					strcpy((char*)ptr->DataType.c_str(),current);
			}

		}

		// store data in queue based on priority queue
		switch (flag)
		{
		case 1:
			Qlow.push(ptr);
			break;
		case 2:
			Qlowest.push(ptr);
			break;
		case 3:
			Qnormal.push(ptr);
			break;
		case 4:
			Qhigh.push(ptr);
			break;
		case 5:
			Qhighest.push(ptr);
			break;
		}
	}
	cout<<"Processed Text File(s) ---->"<<textfile<<endl;
}

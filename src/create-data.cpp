#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#if defined(_MSC_VER)
#include <tchar.h>
#include <strsafe.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#elif defined(__GNUC__) || defined(__GNUG__)
#include <dirent.h>
#endif

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( string& img_filename );

string window_name = "obstacle detection";

int main( int argc, const char** argv )
{
	string dirname;
	if (argc != 2) {
		std::cout << "Usage is:" << endl;
		std::cout << argv[0] << " <dir name>" << std::endl;
		std::cin.get();
		exit(0);
	}
	else {
		dirname = argv[1];
                int l = dirname.length();
                string e = "/";
                string i = dirname.substr(l-1);
                if(i.compare(e) == 0) {
                    dirname = dirname.substr(0, l-1);
                }
	}
	

#if defined(_MSC_VER)

	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(dirname.c_str(), MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		exit(EXIT_FAILURE);
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), dirname.c_str());

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, dirname.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		exit(EXIT_FAILURE);
	}

	// List all the files in the directory with some info about them.
	string img_filename;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
		else
		{
			PTSTR pszFileName = ffd.cFileName;
			std::string name(pszFileName);


			img_filename = dirname + "\\" + name;
			detectAndDisplay(img_filename);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		exit(EXIT_FAILURE);
	}

	FindClose(hFind);

#elif defined(__GNUC__) || defined(__GNUG__)

	cout << "\nTarget directory is " << dirname << "\n\n" << endl;
        DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dirname.c_str())) != NULL) {
		string img_filename;
		while ((ent = readdir(dir)) != NULL) {
                        string n = ent->d_name;	
                        if(n.compare(".") == 0 ||
                           n.compare("..") == 0 ) {
                            continue;
                        }		
			img_filename = dirname + "/" + ent->d_name;
			detectAndDisplay(img_filename);
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}

#endif
	return EXIT_SUCCESS;
}


/**
* @function detectAndDisplay
*/
void detectAndDisplay(string& img_filename)
{
    Mat frame;
    //string image_prefix = "image_inpNN_or_size-Frame";
    //string image_prefix = "mask_prediction-Frame";
    //string image_prefix = "prediction-Frame";
    string image_prefix = "Frame";
    std::size_t found = img_filename.find(image_prefix);
    string i;
    std::stringstream ss;
    ss << found;
    ss >> i;
    cout << i << endl;
    //cout << "aa " << i.compare("p") << endl;
    if (found != std::string::npos && i.compare("19") == 0) {
        cout << img_filename << endl;
        int start = img_filename.find("(");
        int finish = img_filename.find(")");
        string n = img_filename.substr(start+1, finish-start-1);
        cout << n << endl;
        frame = imread(img_filename, CV_LOAD_IMAGE_COLOR);
        if (!frame.data) {
	    cout << "Unable to read input frame: " << img_filename << endl;
	    exit(EXIT_FAILURE);
        }
        imwrite(n+".png", frame);
        //-- Show what you got
        imshow( window_name, frame );
        cvWaitKey(30);            
    }   
}


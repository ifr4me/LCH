
//1. 删除File explorer浏览记录
//2. 删除Mstsc连接记录
//3. 删除PDF、WORD、EXCEL浏览记录

#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>


#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383


#define TOTALBYTES    8192
#define BYTEINCREMENT 4096


void usage(){
	_tprintf(TEXT("********************************************************************************\n List or Clean IE history / Mstsc history / File explorer history / Office03-13 history / Acrobat Reader9 10 11 history.\n Example:%s -l \n  -l List history\n  -d Delete history\n\n Coded by [iframe@T00LS\n\n********************************************************************************"),__argv[0]);
}

void QueryKey(LPTSTR F_record) 
	{ 
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER,F_record,0,KEY_ALL_ACCESS,&hKey)!=ERROR_SUCCESS)
	{
		_tprintf(TEXT("%s\n"),GetLastError());
		return ;
	}
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode; 

	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	//_tprintf( TEXT("Number of subkeys: %d\n"), cSubKeys);

	if (cSubKeys){

		for (i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) 
			{
				_tprintf(TEXT("(%d) %s\n"), i+1, achKey);
			}
		}
	} 

	// Enumerate the key values. 

	_tprintf( TEXT("Number of values: %d\n"), cValues);

	if (cValues) {

		for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
		{ 
			cchValue = MAX_VALUE_NAME; 
			achValue[0] = '\0'; 
			BYTE Key_Data[255];
			DWORD dwSize=255;
			retCode = RegEnumValue(hKey, i, 
				achValue, 
				&cchValue, 
				NULL, 
				NULL,
				Key_Data,
				&dwSize);

			if (retCode == ERROR_SUCCESS ) 
			{ 
				_tprintf(TEXT("(%d) %s \t %s\n"), i+1, achValue,Key_Data); 
			} 
		}
	}
	RegCloseKey(hKey);
}


void DeleteKey(LPTSTR F_record) 
{ 
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER,F_record,0,KEY_ALL_ACCESS,&hKey)!=ERROR_SUCCESS)
	{
		_tprintf(TEXT("%s\n"),"Null");
		return;
	}
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode; 

	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys){
	//_tprintf( TEXT("Number of subkeys: %d\n"), cSubKeys);

	for (i=0; i<cSubKeys;i++){

		cbName = MAX_KEY_LENGTH;
		retCode = RegEnumKeyEx(hKey, 0,
			achKey, 
			&cbName, 
			NULL, 
			NULL, 
			NULL, 
			&ftLastWriteTime); 		

		if (retCode == ERROR_SUCCESS) 
			{
				TCHAR key_str[2]={_T('\0')};
				strncpy(key_str,achKey,1);

				//_tprintf(TEXT("(%d) %s %s\n"), i+1, achKey,key_str);

				if( (_tcscmp(key_str,"c")==0)&&(_tcslen(achKey)>1)&&(_tcslen(achKey)<4))   //
				{
				
					if (RegDeleteKey(hKey,achKey) != ERROR_SUCCESS)
					{
						_tprintf(TEXT("%s %s\n"),GetLastError(),"Delete Key fail!");
					}else{
						_tprintf(TEXT("%s %s\n"),achKey,"has been deleted!");
					}
				}
			}
		}
	} 

	// Enumerate the key values. 

	if (cValues) {
	_tprintf( TEXT("Number of values: %d\n"), cValues);

	//for (i=0;i<cValues; i++){ 
	i=0;
	while(1){ 
		cchValue = MAX_VALUE_NAME; 
		achValue[0] = _T('\0'); 
		BYTE Key_Data[255];
		DWORD dwSize=255;
		retCode = RegEnumValue(hKey, i, 
			achValue, 
			&cchValue, 
			NULL, 
			NULL,
			Key_Data,
			&dwSize);
		if (retCode == ERROR_NO_MORE_ITEMS )
		{
		break;
		}

		TCHAR Value_str[4]={_T('\0')};
		strncpy( Value_str,achValue,3);

		if(_tcscmp( Value_str,"Ite")==0 ){RegDeleteValue(hKey,achValue);_tprintf(TEXT(" %s %s %s\n"), achValue,Key_Data,"has been deleted!"); }
		if(_tcscmp( Value_str,"MRU")==0 ){RegDeleteValue(hKey,achValue);_tprintf(TEXT(" %s %s %s\n"), achValue,Key_Data,"has been deleted!"); }
		if(_tcscmp( Value_str,"url")==0 ){RegDeleteValue(hKey,achValue);_tprintf(TEXT(" %s %s %s\n"), achValue,Key_Data,"has been deleted!"); }

		}
	}
	RegCloseKey(hKey);
}


int _tmain(int argc, char *argv[])
{	if(argc!=2){usage();exit(0);}	
//	((argv[1]=="-l")||(argv[1]=="-d")&&(int(argv[2])<4)&&(int(argv[2])>0))
//1 File explorer   2 Word  3 Excel 4 Access    3 PDF OFFICE Mstsc

	LPTSTR F_record ;
	
	if(_tcscmp(argv[1],"-d") ==0)
	{
		F_record = TEXT("Software\\Microsoft\\Internet Explorer\\TypedURLs");
		//_tprintf("%s\n",F_record);	
		_tprintf(TEXT("Internet Explorer:\n"));
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\TypedPaths");
		//_tprintf("%s\n",F_record);
		_tprintf(TEXT("\nExplorer:\n"));
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Terminal Server Client\\Default");
		//_tprintf(TEXT("%s\n"),F_record);
		_tprintf(TEXT("\nMSTSC:\n"));
		DeleteKey(F_record) ;
		if(DeleteFile("%USERPROFILE%\\My Documents\\default.rdp"))
			{
				_tprintf(TEXT("%s\n"),"%USERPROFILE%\\My Documents\\default.rdp has deleted!");
		}else{
				if(DeleteFile("%USERPROFILE%\\Documents\\default.rdp")){_tprintf(TEXT("%s\n"),"%USERPROFILE%\\Documents\\default.rdp has deleted!");}
		}

		_tprintf(TEXT("\n%s\n"),"Office2003");

		F_record = TEXT("Software\\Microsoft\\Office\\11.0\\Word\\Recent Files");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\11.0\\Excel\\Recent Files");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\11.0\\PowerPoint\\Recent Files");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		_tprintf(TEXT("\n%s\n"),"Office2007");

		F_record = TEXT("Software\\Microsoft\\Office\\12.0\\Word\\File MRU");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\12.0\\Excel\\File MRU");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\12.0\\PowerPoint\\File MRU");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		_tprintf(TEXT("\n%s\n"),"Office2010");

		F_record = TEXT("Software\\Microsoft\\Office\\14.0\\Word\\Place MRU");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\14.0\\Excel\\Place MRU");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\14.0\\PowerPoint\\Place MRU");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		_tprintf(TEXT("\n%s\n"),"Office2013");

		F_record = TEXT("Software\\Microsoft\\Office\\15.0\\Word\\User MRU");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\15.0\\Excel\\User MRU");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\15.0\\PowerPoint\\User MRU");
		_tprintf("%s\n",F_record);

		_tprintf(TEXT("\n%s\n"),"Acrobat Reader9 10 11");

		DeleteKey(F_record) ;
		F_record = TEXT("Software\\Adobe\\Acrobat Reader\\9.0\\AVGeneral\\cRecent");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Adobe\\Acrobat Reader\\10.0\\AVGeneral\\cRecentFiles");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record) ;

		F_record = TEXT("Software\\Adobe\\Acrobat Reader\\11.0\\AVGeneral\\cRecentFiles");
		//_tprintf("%s\n",F_record);
		DeleteKey(F_record);
	}

	if(_tcscmp(argv[1],"-l") ==0)
	{
		F_record = TEXT("Software\\Microsoft\\Internet Explorer\\TypedURLs");
		//_tprintf(TEXT("%s\n"),F_record);
		_tprintf(TEXT("Internet Explorer:\n"));
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\TypedPaths");
		//_tprintf("%s\n",F_record);
		_tprintf(TEXT("\nExplorer:\n"));
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Terminal Server Client\\Default");
		//_tprintf(TEXT("%s\n"),F_record);	
		_tprintf(TEXT("\nMSTSC:\n"));
		QueryKey(F_record) ;

		_tprintf(TEXT("\n%s\n"),"Office2003");

		F_record = TEXT("Software\\Microsoft\\Office\\11.0\\Word\\Recent Files");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\11.0\\Excel\\Recent Files");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\11.0\\PowerPoint\\Recent Files");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		_tprintf(TEXT("\n%s\n"),"Office2007");

		F_record = TEXT("Software\\Microsoft\\Office\\12.0\\Word\\File MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\12.0\\Excel\\File MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\12.0\\PowerPoint\\File MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		_tprintf(TEXT("\n%s\n"),"Office2010");

		F_record = TEXT("Software\\Microsoft\\Office\\14.0\\Word\\Place MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\14.0\\Excel\\Place MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\14.0\\PowerPoint\\Place MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		_tprintf(TEXT("\n%s\n"),"Office2013");

		F_record = TEXT("Software\\Microsoft\\Office\\15.0\\Word\\User MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\15.0\\Excel\\User MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		F_record = TEXT("Software\\Microsoft\\Office\\15.0\\PowerPoint\\User MRU");
		//_tprintf("%s\n",F_record);
		QueryKey(F_record) ;

		_tprintf(TEXT("\n%s\n"),"Acrobat Reader9 10 11");

		 F_record = TEXT("Software\\Adobe\\Acrobat Reader\\9.0\\AVGeneral\\cRecent");
		 //_tprintf("%s\n",F_record);
		 QueryKey(F_record) ;

		 F_record = TEXT("Software\\Adobe\\Acrobat Reader\\10.0\\AVGeneral\\cRecentFiles");
		// _tprintf("%s\n",F_record);
		 QueryKey(F_record) ;

		 F_record = TEXT("Software\\Adobe\\Acrobat Reader\\11.0\\AVGeneral\\cRecentFiles");
		 //_tprintf("%s\n",F_record);
		 QueryKey(F_record) ;
	}

	//_tprintf("%d\n",argc);
	//while(argc){_tprintf("%s ",argv[--argc]);}
	return 0;
}


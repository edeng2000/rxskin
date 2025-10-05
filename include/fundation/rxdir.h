#ifndef ___RX_DIR_H____
#define ___RX_DIR_H____
/**
* @file sddir.h
* @author lw
* @brief dir utility
*
**/
#if (defined(WIN32) || defined(WIN64))
#else
#include <sys/types.h>
#include <dirent.h>
#endif
#include <string>
#include "rxtype.h"
#include "rxfile.h"

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#else
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir    
#endif


namespace RX
{
    /**
    * @defgroup groupdir Ŀ¼����
    * @ingroup  GNDP
    * @{
    */

    struct SFileAttr
    {
        BOOL isDir; //�Ƿ�ΪĿ¼
    };
    /**
    *@brief Ŀ¼�����࣬�����ڻ�ȡ��Ŀ¼�����е��ļ����ļ��е�����
    */
    class CRXDirectory
    {
    public:

        CRXDirectory();
        ~CRXDirectory();
        /**
        * @brief
        * ��һ��Ŀ¼��������fopenһ��
        * @param pszPath : Ŀ¼��·��
        * @return FALSE �����ʧ�ܣ�TRUE����򿪳ɹ�
        */
        BOOL  Open(const TCHAR* pszPath);

        /**
        * @brief
        * �رմ�Ŀ¼
        * @return VOID
        */
        VOID  Close();

        /**
        * @brief
        * ��ȡ��Ŀ¼����һ���ļ������ƣ�����֪�����ļ��Ƿ�ΪĿ¼
        * @param pszName : [�������]�Ӹ�Ŀ¼�л�ȡ���ļ���
        * @param nBufLen : [�������]�ܹ���ֵ����󳤶�
        * @param pAttr : [�������]�����ļ�����, isDir���Ϊ1����ΪĿ¼������Ϊ0����������ΪNULL�����޷���ֵ
        * @return �ɹ�����TRUE����ȡʧ�ܣ����ߵ���Ŀ¼���ļ��Ľ�β����FALSE
        */
        BOOL  Read(TCHAR *pszName, INT32 nBufLen, SFileAttr *pAttr = NULL);

    private:
#if (defined(WIN32) || defined(WIN64))
        tstring m_szPath;
        HANDLE m_fHandle;
#else
        DIR * m_dir;
		CHAR m_curDir[SD_MAX_PATH]; 
#endif
    };


    /**
    * @brief
    * ����Ŀ¼
    * @param pszDirName : ��������Ŀ¼·��������
    * @param bforce : ��û���ϼ�Ŀ¼ʱ���Ƿ�ǿ�ƴ���Ŀ¼
    * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
    */
    BOOL  RXCreateDirectory(const TCHAR *pszDirName, BOOL bForce= FALSE);
    BOOL  RXRenameDirectory(const TCHAR* pszDirName,const TCHAR* pszNewDirName);
    /**
    * @brief
    * ɾ��һ��Ŀ¼
    * @param pszDirName : ��ɾ����Ŀ¼·��������
    * @param bForce     : �Ƿ�ǿ��ɾ��Ŀ¼
    * @return �ɹ�����TRUE��ʧ�ܷ���FALSE(�ļ����������ݻ���û��ɾ��Ȩ��)
    */
    BOOL  RXDeleteDirectory(const TCHAR *pszDirName, BOOL bForce = FALSE);

    /**
    * @brief
    * ɾ��һ��Ŀ¼
    * @param pszDirName : ��ɾ����Ŀ¼·��������
    * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
    */
    BOOL  RXDirectoryExists(const TCHAR *pszDirName);

	/**
	* @brief
	* �ж�һ���ļ��Ƿ�Ϊ�ļ���
	* @param pszFileName : �ļ���
	* @return �ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
    BOOL  RXIsDirectory(const TCHAR * pszFileName);

	/**
	* @brief
	* �ж�һ���ļ����Ƿ�Ϊ��
	* @param pszFileName :  �ļ���
	* @return �ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
    BOOL  RXIsEmptyDirectory(const TCHAR* pszFileName);

	//TODO Ŀ¼����
    void RXGetAllFiles(TCHAR* lpPath, std::vector<tstring>& fileList);
    void RXGetAllDirectory(TCHAR* lpPath, std::vector<tstring>& fileList);

	//TODO Ŀ¼����
	void RXGetAllFilesEx(const TCHAR* lpPath, const TCHAR* pszExt, std::vector<tstring>& fileList);


    struct SFileInfo
    {
        BOOL  bIsFile;      
        FILETIME  llTime;
        TCHAR szFullPath[MAX_PATH];
        TCHAR szName[MAX_PATH];
        SFileInfo();
        SFileInfo(const SFileInfo& oInfo);
    };

	//TODO Ŀ¼����
	void RXGetAllFileName(const TCHAR* lpPath, std::vector<tstring>& fileList);
	void RXGetCurDirAllFileName(const TCHAR* lpPath, std::vector<SFileInfo*>& fileList, std::vector<SFileInfo*>& dirList);

    tstring RXGetUpDirectory(const TCHAR* pszDir);
    tstring RXGetUpDirectoryForMac(const TCHAR* pszDir);

    /** @} */
}

#endif



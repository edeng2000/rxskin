#ifndef ___RX__FILE_H____
#define ___RX__FILE_H____

#include "rxtype.h"
#include <stdio.h>
#include <string>
#include <time.h>
#include <vector>
#include "rxstring.h"
#if (defined(WIN32) || defined(WIN64))
#else
#include <sys/stat.h>
#include <fcntl.h>
#endif
/**
* @file sdfile.h
* @author lw
* @brief �ļ�������
*
**/

namespace RX
{
    /**
    * @defgroup groupfile �ļ�����
    * @ingroup  GNDP
    * @{
    */

    /* An arbitrary size that is digestable. True max is a bit less than 32000 */
#if (defined(WIN32) || defined(WIN64))
#define RX_MAX_PATH  MAX_PATH
#define RXPATH_DELIMITER _T('\\')
#else /* !SD_HAS_UNICODE_FS */
#define SD_MAX_PATH 260
#define SDPATH_DELIMITER '/'
#endif

    /* Win32 .h ommissions we really need */
#define RXFILENO_STDIN   0
#define RXFILENO_STDOUT  1
#define RXFILENO_STDERR  2
#define RXPATH_IS_DELIMITER(x)  (x == _T('\\') || x == _T('/'))
    /**
    *@brief �ļ�״̬�ṹ��
    */
    struct SFileStatus
    {
        UINT32 mode;		/**<�ļ���Ӧ��ģʽ���ļ���Ŀ¼��*/
        UINT32 inode;		/**<i-node�ڵ��, linux-specific*/
        UINT32 dev;			/**<�豸����*/
        UINT32 rdev;		/**<�����豸����*/
        UINT32 nlink;		/**<�ļ���������*/
        UINT32 uid;			/**<�ļ�������, linux-specific*/
        UINT32 gid;			/**<�ļ������߶�Ӧ����, linux-specific*/
        UINT32 size;		/**<��ͨ�ļ�����Ӧ���ļ��ֽ���*/
        time_t atime;		/**<�ļ���󱻷��ʵ�ʱ��*/
        time_t mtime;		/**<�ļ���������޸ĵ�ʱ��*/
        time_t ctime;		/**<�ļ�״̬�����ԣ��ı�ʱ��*/
    };


    /**
    *@brief �ļ�Ȩ��ö����
    */
    enum AccessMode
    {
#if (defined(WIN32) || defined(WIN64))
        EXIST_OK = 0,
        RD_OK = 2,
        WD_OK = 4,
#else
        EXIST_OK = F_OK,
        RD_OK = R_OK,
        WD_OK = W_OK,
        EXECUTE_OK = X_OK,
#endif
    };



    /**
    *@brief �ļ������࣬�����������ļ�����,��ʹ��FileMapping��ʽ��ȡ�ļ�(���幦�ܲ��սӿ�)
    */
    class CRXFile
    {

    public:

		/**
		* @brief ���캯������ʼ���ļ�������ļ��ڴ�ӳ��
		*/
        CRXFile();

		/**
		* @brief �����������ر��ļ����
		*/
        ~CRXFile();

        /**
        *@brief seekλ��ö����
        */
        enum SeekOffset
        {
            SK_SET = SEEK_SET,		/**<�ļ���ʼλ��*/
            SK_END = SEEK_END,		/**<�ļ���βλ��*/
            SK_CUR = SEEK_CUR,		/**<�ļ���ǰλ��*/
        };

        /**
        * @brief
        * Open a file, type����ͬfopen�ӿ�
        * @param pszFileName : �ļ�����
        * @param type : �ļ�����Ȩ��
        * @param bFileMapping : �Ƿ��ļ�ӳ�䵽�ڴ�
        * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
        */
        BOOL  Open(const TCHAR* pszFileName, const TCHAR* type, BOOL bFileMapping = FALSE);

        /**
        * @brief
        * �ر�һ���ļ�
        * @return VOID
        */
        VOID  Close();

        /**
        * @brief
        * Flushˢ���ļ�
        * @return VOID
        */
        VOID  Flush();

        /**
        * @brief
        * ���ļ��ж�ȡ����
        * @param pBuf : ��ȡ���ݵĴ��λ��
        * @param nLen : �ܹ��淢���ݵ���󳤶�
        * @return ʵ�ʶ�ȡ�����ݳ���
        */
        UINT32  Read(VOID* pBuf, UINT32 nLen);

        /**
        * @brief
        * ���ļ���д������
        * @param pBuf : ָ��д�����ݵ�ָ��
        * @param nLen : ����д�����ݳ���
        * @return ʵ��д������ݳ���
        */
        UINT32  Write(const VOID* pBuf, UINT32 nLen);

        /**
        * @brief
        * �ƶ��ļ��α굽�ض���λ��
        * @param offset : ��Գ�ʼλ���ƶ��ľ���
        * @param whence : ��ʼλ�ã�������enum SeekOffset�е�ֵ
        * @return ����ɹ�������0
        */
        INT32  Seek(INT32 offset, CRXFile::SeekOffset whence);

        /**
        * @brief
        * �ƶ��ļ��α굽��ǰ�ļ����ļ�ͷ
        * @return ���أ�1����ʧ��
        */
        INT32  SeekToBegin();

        /**
        * @brief
        * �ƶ��ļ��α굽��ǰ�ļ����ļ�β
        * @return ���أ�1����ʧ��
        */
        INT32  SeekToEnd();

        /**
        * @brief
        * ��ȡ��ǰ�ļ��α�����ļ�ͷ�ľ��룬��λbyte
        * @return ��ǰ�ļ��α�����ļ�ͷ�ľ���
        */
        LONG  GetPosition();

        /**
        * @brief
        * �����ļ��ĳ��ȣ���λΪbyte
        * @param newLen : ���µ��ļ����ȣ���λΪbyte
        * @return VOID
        */
        VOID  SetLength(LONG newLen);

        /**
        * @brief
        * �����Ƿ�Ϊ�ļ�β��
        * @return ������ļ�β��������TRUE�����򷵻�FALSE
        */
        BOOL  Eof();

        /**
        * @brief
        * ��ȡ��ǰ�ļ�������
        * @param pStatus : ��ȡ���ļ����Խṹ��
        * @return ����ɹ�������TRUE�����򷵻�FALSE
        */
        BOOL  GetFileStatus(SFileStatus &pStatus);

    private:

		/**
		* @brief �������캯��
		* @param origFile : CSDFile���������
		*/
        CRXFile(const CRXFile& origFile);              // no implementation
        
		/**
		* @brief ����=��������ʵ�ֶ���ĸ�ֵ
		* @param origFile : CSDFile���������
		* @return VOID
		*/
		VOID operator=(const CRXFile& origFile);       // no implementation

        VOID * m_pFileHandle;
        tstring m_filepath;
        BOOL m_bFileMapping; // �Ƿ�ʹ��FileMapping��ʽ��ȡ�ļ�
    };


    /**
    * @brief
    * ��ȡ�ļ�������
    * @param pszFileName : ��Ҫ��ȡ�ļ����Ե��ļ�·��
    * @param pStatus : ��ȡ���ļ����Խṹ��
    * @return ����ɹ�������TRUE�����򷵻�FALSE
    */
    BOOL  RXGetFileStatus(const TCHAR* pszFileName, SFileStatus& stStatus);

    uint64_t RXFileSize(const TCHAR* pszFileName);
    /**
    * @brief
    * ɾ��һ���ļ�
    * @param pszFileName : ��Ҫɾ�����ļ����ļ�·��
    * @return �Ƿ�ɹ�������TRUE�����򷵻�FALSE
    */
    BOOL  RXFileRemove(const TCHAR* pszFileName);

    /**
    * @brief
    * ����һ���ļ����ļ������������ļ�·����
    * @param pszOldName : ԭ�ȵ��ļ�·��
    * @param pszNewName : �µ��ļ�·��
    * @param bForce     : �Ƿ�ǿ���������ļ�
    * @return �ɹ�����0
    */
    INT32  RXFileRename(const TCHAR *pszOldName, const TCHAR *pszNewName, BOOL bForce = FALSE);

    /**
    * @brief
    * ����һ���ļ����ļ�·�����������ļ�����
    * @param pszOldName : ԭ�ȵ��ļ�·��
    * @param pszNewName : �µ��ļ�·��
    * @param bForce     : �Ƿ�ǿ���ƶ��ļ�
    * @return �ɹ�����0
    */
    INT32  RXFileMove(const TCHAR *pszOldName, const TCHAR *pszNewName, BOOL bForce = FALSE);

    /**
    * @brief
    * ȷ���ļ��Ķ�дȨ��
    * @param pszPath : ��Ҫȷ����дȨ�޵��ļ�·��
    * @param mode : ��Ҫȷ�ϵĿɷ���ģʽ
    * �����¼���ģʽ��EXIST_OK(�Ƿ����), RD_OK(�Ƿ�ɶ�), WD_OK(�Ƿ��д)
    * @return ���ӵ�и�ģʽ������0
    */
    INT32  RXFileAccess(const TCHAR *pszPath, AccessMode mode);

    /**
    * @brief
    * ��ȡ��ǰӦ�õ�����·��,����ִ���ļ����ļ���
    * ��Ҫע����ǣ��˺����������������Ҳ�޷�ȷ���̰߳�ȫ
    * @return ��ȡ�ĵ�ǰӦ�õ�·��
    */
    const TCHAR*  RXGetModuleName();

    /**
    * @brief
    * ��ȡ��ǰӦ�õ�·��
    * ��Ҫע����ǣ��˺����������������Ҳ�޷�ȷ���̰߳�ȫ
    * @return ��ǰӦ�õ�·��
    */
    const TCHAR*  RXGetModulePath();

	/**
	* @brief
	* ��ȡ��ǰӦ�õĹ���·��(current work directory) 
	* ��Ҫע����ǣ��˺����������������Ҳ�޷�ȷ���̰߳�ȫ
	* @return ��ǰӦ�õĹ���·��
	*/
    const TCHAR*  RXGetWorkPath();

    /**
    * @brief
    * ��ȡ�ļ����ļ�·��
    * @param pszFileName : �ļ�·�����ļ�����
    * @return �ļ�·��
    */
    tstring  RXFileExtractPath(const TCHAR *pszFileName);

    /**
    * @brief
    * ��ȡ�ļ����ļ���
    * @param pszFileName : �ļ�·�����ļ�����
    * @return �ļ���
    */
    tstring  RXFileExtractName(const TCHAR *pszFileName);

    /**
    * @brief
    * ��ȡ�ļ����ļ���չ��
    * @param pszFileName : �ļ�·�����ļ�����
    * @return �ļ���չ��
    */
    tstring  RXFileExtractExt(const TCHAR *pszFileName);

    /**
    * @brief
    * ȥ���ļ�·������"\"
    * @param pszPath : �ļ�·��
    * @return ȥ������"\"����ļ�·��
    */
    tstring  RXFileExcludeTrailingDelimiter(const TCHAR *pszPath);

    /**
    * @brief
    * ���"\"���ļ�·��������ļ�·������"\"�������
    * @param pszPath : �ļ�·��
    * @return ���"\"����ļ�·��
    */
    tstring  RXFileIncludeTrailingDelimiter(const TCHAR *pszPath);

    /**
    * @brief
    * ���"\"���ļ�·��ǰ�����ļ�·��ǰ����"\"�������
    * @param pszPath : �ļ�·��
    * @return ���"\"����ļ�·��
    */
    tstring  RXFileIncludeLeadingDelimiter(const TCHAR *pszPath);

    /**
    * @brief
    * ȥ���ļ�·��ǰ��"\"
    * @param pszPath : �ļ�·��
    * @return ȥ��"\"����ļ�·��
    */
    tstring  RXFileExcludeLeadingDelimiter(const TCHAR *pszPath);

    /**
    * @brief
    * ���ļ�·���е�"\"��"//"�滻Ϊ�ض����ַ�delimiter
    * @param pszPath : �ļ�·��
    * @param delimiter : �滻�ַ�
    * @return �滻����ļ�·��
    */
    tstring  RXFileReplaceDelimiter(const TCHAR *pszPath, TCHAR delimiter);

    /**
    * @brief
    * ����ļ��Ƿ����
    * @param pszFileName : �ļ�·��
    * @return �����򷵻�TRUE���������򷵻�FALSE
    */
    BOOL  RXFileExists(const TCHAR *pszFileName);

    /**
    * @brief
    * ��ȡ�ļ���С
    * @param pszFileName : �ļ�·��
    * @return �ļ���С
    */
    uint64_t  RXFileGetSize(const TCHAR *pszFileName);

    /**
    * @brief
    * �����ļ�
    * @param pszFileName : �ļ�·��
    * @return �����ɹ�����TRUE�����򷵻�FALSE
    */
    BOOL  RXFileCreate(const TCHAR *pszFileName);

	/**
	* @brief
	* �����ļ�
	* @param pszExistFile :Դ�ļ�·�� 
	* @param pNewFile :   Ŀ���ļ�·��
	* @param bFailIfExists : ���Ŀ������Ƿ񷵻�ʧ��
	* @return �����ɹ�����TRUE�����򷵻�FALSE
	*/
	BOOL  RXFileCopy(const TCHAR *pszExistFile, const TCHAR *pNewFile, BOOL bFailIfExists); 

    BOOL RXSaveBmp(const TCHAR* pszFilePath, const BYTE* imgBuffer, const uint32_t& dwWidth, const uint32_t& dwHeight, const uint32_t& dwBitcount);


    /** @} */
}

#endif






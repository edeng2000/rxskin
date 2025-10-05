/******************************************************************************
Copyright (C) Shanda Corporation. All rights reserved.


******************************************************************************/

#ifndef SDXMLFILE_H
#define SDXMLFILE_H
#include <windows.h>
#include <string>
#include <vector>

#include "fundation/rxfundation.h"
using namespace std;
/**
* @brief xml�ڵ��࣬���ڼ�¼xml�ļ��ж����Ľڵ�
*/
class RXFUNDATION_API CRXXMLNode
{
    /**
    * @brief ��¼xml�ļ��нڵ����Ե�ֵ
    */
    class RXFUNDATION_API CXMLValue
    {
    public:

		/**
		* @brief ���캯������ʼ���ڵ����Ե�ֵ
		*/
        CXMLValue();

		/**
		* @brief ���캯��
		* @param pszValue : Ҫ���õ��ַ���ֵ
		*/
        CXMLValue(const CHAR* pszValue);

		/**
		* @brief �������캯��
		* @param v : CXMLValue���������
		*/
        CXMLValue(const CXMLValue& v);

        /**
        * @brief ���¸�������ֵ�����ͣ����ַ���ת��Ϊ��ͬ���͵�����ֵ
        * @param pszString : �ַ������� 
        * @return ���غͲ�����Ӧ���͵�ֵ
        */
        std::string AsString(const CHAR* pszString = "");

		/**
        * @brief ���¸�������ֵ�����ͣ����ַ���ת��Ϊ��ͬ���͵�����ֵ
        * @param nInteger : INT32���͵�ֵ 
        * @return ���غͲ�����Ӧ���͵�ֵ
        */
        INT32 AsInteger(INT32 nInteger = 1);

		/**
        * @brief ���¸�������ֵ�����ͣ����ַ���ת��Ϊ��ͬ���͵�����ֵ
        * @param fFloat : FLOAT���͵�ֵ 
        * @return ���غͲ�����Ӧ���͵�ֵ
        */
        FLOAT AsFloat(FLOAT fFloat = 1.0);

		/**
        * @brief ���¸�������ֵ�����ͣ����ַ���ת��Ϊ��ͬ���͵�����ֵ
        * @param bBoolean : BOOL���͵�ֵ 
        * @return ���غͲ�����Ӧ���͵�ֵ
        */
        BOOL  AsBoolean(BOOL bBoolean = FALSE);

		/**
        * @brief ���¸�������ֵ�����ͣ����ַ���ת��Ϊ��ͬ���͵�����ֵ
        * @param dDouble : DOUBLE���͵�ֵ 
        * @return ���غͲ�����Ӧ���͵�ֵ
        */
        DOUBLE AsDouble(DOUBLE dDouble = 1.00);
    protected:
        const CHAR* m_pszValue;
    };
public:

	/**
	* @brief ���캯������ʼ��xml�ڵ�Ԫ��
	*/
    CRXXMLNode();

	/**
	* @brief �������캯��
	* @param xmlNode : CRXXMLNode���������
	*/
    CRXXMLNode(const CRXXMLNode& xmlNode);

    /**
    * @brief ����[]�����������ݽڵ����õ��ڵ㣨�������·������ʽ��
    * @param pszNodeName : �ڵ������
    * @return xml�ڵ�
    */
    CRXXMLNode operator [] (const CHAR* pszNodeName);

    /**
    * @brief ����()���������õ���ǰ�ڵ�ĳһ���Ե�ֵ
    * @param pszAttrName : ��������
    * @return CXMLValue���󣬼����Ե�ֵ
    */
    CXMLValue operator ()(const CHAR* pszAttrName);

    /**
    * @brief ����ǰ�ڵ���NULL���бȽϣ��ж��Ƿ񲻵ȣ�������ָ��������
    * @param node : xml�ڵ㣨Ĭ�ϲ���ΪNULL��
    * @return TRUE/FALSE
    */
    BOOL operator != ( const CRXXMLNode* node );

	/**
    * @brief ����ǰ�ڵ���NULL���бȽϣ��ж��Ƿ���ȣ�������ָ��������
    * @param node : xml�ڵ㣨Ĭ�ϲ���ΪNULL��
    * @return TRUE/FALSE
    */
    BOOL operator == ( const CRXXMLNode* node );

    /**
    * @brief ���¸���ֵ�����ͣ������ǰ�ڵ��ֵ����õ��ַ���������Ҫ�����͵�ת����
    * @param pszString : �ַ���
    * @return ��Ӧ���͵�ֵ
    */
    std::string AsString(const CHAR* pszString = "");

	/**
    * @brief ���¸���ֵ�����ͣ������ǰ�ڵ��ֵ����õ��ַ���������Ҫ�����͵�ת����
    * @param nInteger : INT32���͵�ֵ
    * @return ��Ӧ���͵�ֵ
    */
    INT32 AsInteger(INT32 nInteger = 1);

	/**
    * @brief ���¸���ֵ�����ͣ������ǰ�ڵ��ֵ����õ��ַ���������Ҫ�����͵�ת����
    * @param fFloat : FLOAT���͵�ֵ
    * @return ��Ӧ���͵�ֵ
    */
    FLOAT AsFloat(FLOAT fFloat = 1.0);

	/**
    * @brief ���¸���ֵ�����ͣ������ǰ�ڵ��ֵ����õ��ַ���������Ҫ�����͵�ת����
    * @param bBoolean : BOOL���͵�ֵ
    * @return ��Ӧ���͵�ֵ
    */
    BOOL  AsBoolean(BOOL bBoolean = FALSE);

	/**
    * @brief ���¸���ֵ�����ͣ������ǰ�ڵ��ֵ����õ��ַ���������Ҫ�����͵�ת����
    * @param dDouble : DOUBLE���͵�ֵ
    * @return ��Ӧ���͵�ֵ
    */
    DOUBLE AsDouble(DOUBLE dDouble = 1.00);

/**
* @brief ��ȡ��ǰ�ڵ�ĵ�һ���ֵܽڵ�
* @return xml�ڵ�
*/
    CRXXMLNode Sibling();

/**
* @brief ���ݽڵ����ֻ�ȡ��ǰ�ڵ���ֵܽڵ�
* @param szNext : ��ǰ�ڵ���ֵܽڵ�����
* @return xml�ڵ�
*/
    CRXXMLNode Sibling(const CHAR* szNext);

    /**
    * @brief ����xmlԪ�ص�ֵ
    * @param tempElement : xmlԪ��
    * @return VOID
    */
    VOID SetElement(VOID* tempElement)
    {
        m_pElement = tempElement;
    }
private:
    VOID*   m_pElement;
};

/**
* @brief xml�ļ��࣬��xml�ڵ�������
*/
class RXFUNDATION_API CRXXMLFile:public CRXXMLNode
{
public:

	/**
	* @brief ���캯������ʼ��xml�ĵ�����
	*/
    CRXXMLFile();

	/**
	* @brief �����������ͷ�xml�ĵ�����
	*/
    ~CRXXMLFile();

	/**
	* @brief ���캯��
	* @param pszXmlFile : xml�ļ���
	*/
    CRXXMLFile(const CHAR* pszXmlFile);

    /**
    * @brief ����xml�ļ�
    * @param pszXmlFile : xml�ļ���
    * @return TRUE:���سɹ� FALSE:����ʧ��
    */
    BOOL Load(const CHAR* pszXmlFile);
	BOOL Load(FILE* hFile);
    /**
    * @brief ����xml�ĵ�����
    * @return VOID
    */
    VOID UnLoad();

    /**
    * @brief ��ȡxml�ļ��ĸ��ڵ�
    * @return xml�ڵ����
    */
    CRXXMLNode GetRootNode();

    /**
    * @brief �ж�xml�ĵ������Ƿ���Ч
    * @return TRUE:��Ч FALSE:��Ч
    */
    BOOL       IsValid();
protected:
    VOID*	m_pDocument;
    
};

#endif


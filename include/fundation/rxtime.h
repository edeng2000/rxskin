/******************************************************************************
Copyright (C) Shanda Corporation. All rights reserved.

sdtime.h - ʱ�䴦����
******************************************************************************/

#ifndef RXTIME_H
#define RXTIME_H

#include "rxtype.h"

#include <ctime>
#include <string>

#ifndef tstring
#if (defined(WIN32) || defined(WIN64))

#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#else
#ifdef UNICODE
typedef std::basic_string<TCHAR> tstring;
#else
typedef std::string tstring;
#endif
#endif
#endif
/**
* @file sdtime.h
* @author lw
* @brief ʱ�䴦��ϵ��
*
**/

    /**
    * @defgroup grouptime ʱ�����
    * @ingroup  GNDP
    * @{
    */
    class CRXDateTime;

    /**
    * @brief ��ȡ����ǰ����ʱ��(���ô˺����Ĵ�ʱ�˿�)��CRXDateTime
    * @return ����ǰ����ʱ��(���ô˺����Ĵ�ʱ�˿�)��CRXDateTime
    */

    CRXDateTime RXNow();

    /**
    * @brief ����ʱ����,����1970����ҹ��2037��֮���ĳһ��ʱ��
    *
    */
    class  CRXDateTime
    {
    public:
        /**
        * @brief ����һ��CRXDateTime��,���������ʱ��Ϊ1970-1-1��ҹ
		* @param t : ���õ�����ʱ��
        */
        CRXDateTime(time_t t = 0);


        /**
        * @brief ��ȡ���������������ʱ���time_tֵ
        * @return ���ص�time_tֵ
        */
        time_t GetTimeValue();

        /**
        * @brief ���ñ��������������ʱ��
        * @param t ���õ�����ʱ��
        * @return VOID
        */
        VOID SetTimeValue(time_t t);

        /**
        * @brief ��ȡ���������������ʱ����datetime�����������ʱ���������
        * @param datetime һ������ʱ��
        * @return ��������
        */
        INT64 operator - (const CRXDateTime &datetime);

        /**
        * @brief ��ȡ���������������ʱ����datetime�����������ʱ���������
        * @param datetime һ������ʱ��
        * @return ��������
        */
        INT64 DiffSecond(const CRXDateTime &datetime);

        /**
        * @brief ��ȡ���������������ʱ����datetime�����������ʱ����ķ�����,�������1���ӵĲ���������
        * @param datetime һ������ʱ��
        * @return ���ķ�����
        */
        INT64 DiffMinute(const CRXDateTime &datetime);

        /**
        * @brief ��ȡ���������������ʱ����datetime�����������ʱ�����Сʱ��,�������1Сʱ�Ĳ���������
        * @param datetime һ������ʱ��
        * @return ����Сʱ��
        */
        INT64 DiffHour(const CRXDateTime &datetime);

        /**
        * @brief ��ȡ���������������ʱ����datetime�����������ʱ���������,�������1��Ĳ���������
        * @param datetime һ������ʱ��
        * @return ��������
        */
        INT64 DiffDay(const CRXDateTime &datetime);

        /**
        * @brief ��ȡ���������������ʱ����datetime�����������ʱ�����������,�������1���ڵĲ���������
        * @param datetime һ������ʱ��
        * @return ����������
        */
        INT64 DiffWeek(const CRXDateTime &datetime);

        /**
        * @brief ��ȡ���������������ʱ����datetime�����������ʱ������·�,�������1���µĲ���������
        * @param datetime һ������ʱ��
        * @return ����������
        */
        INT32 DiffMonth(const CRXDateTime &datetime);

        /**
        * @brief ��ȡ���������������ʱ����datetime�����������ʱ���������,�������1��Ĳ���������
        * @param datetime һ������ʱ��
        * @return ��������
        */
        INT32 DiffYear(const CRXDateTime &datetime);

        /**
        * @brief ���ӱ��������������ʱ�������
        * @param year ���ӵ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime& IncYear(UINT32 year = 1);

        /**
        * @brief ���ٱ��������������ʱ�������
        * @param year ���ٵ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime& DecYear(UINT32 year = 1);

        /**
        * @brief ���ӱ��������������ʱ�������
        * @param month ���ӵ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime& IncMonth(UINT32 month = 1);

        /**
        * @brief ���ٱ��������������ʱ�������
        * @param month ���ٵ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime& DecMonth(UINT32 month = 1);

        /**
        * @brief ���ӱ��������������ʱ�������
        * @param day ���ӵ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime& IncDay(UINT32 day = 1);

        /**
        * @brief ���ٱ��������������ʱ�������
        * @param day ���ٵ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime& DecDay(UINT32 day = 1);

        /**
        * @brief ���ӱ��������������ʱ���Сʱ��
        * @param hour ���ӵ�Сʱ��
        * @return �µ�����ʱ��
        */
        CRXDateTime& IncHour(UINT32 hour = 1);

        /**
        * @brief ���ٱ��������������ʱ���Сʱ��
        * @param hour ���ٵ�Сʱ��
        * @return �µ�����ʱ��
        */
        CRXDateTime & DecHour(UINT32 hour = 1);

        /**
        * @brief ���ӱ��������������ʱ��ķ�����
        * @param minute ���ӵķ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime & IncMinute(UINT32 minute = 1);

        /**
        * @brief ���ٱ��������������ʱ��ķ�����
        * @param minute ���ٵķ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime & DecMinute(UINT32 minute = 1);

        /**
        * @brief ���ӱ��������������ʱ�������
        * @param second ���ӵ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime & IncSecond(UINT32 second = 1);

        /**
        * @brief ���ٱ��������������ʱ�������
        * @param second ���ٵ�����
        * @return �µ�����ʱ��
        */
        CRXDateTime & DecSecond(UINT32 second = 1);

        /**
        * @brief ���ӱ��������������ʱ���������
        * @param week ���ӵ�������
        * @return �µ�����ʱ��
        */
        CRXDateTime & IncWeek(UINT32 week = 1);

        /**
        * @brief ���ٱ��������������ʱ���������
        * @param week ���ٵ�������
        * @return �µ�����ʱ��
        */
        CRXDateTime & DecWeek(UINT32 week = 1);



        /**
        * @brief ���ñ��������������ʱ��
        * @param year ���õ����[1970-2037]
        * @param month �趨�Ĵ�����ĸ���[1-12]
        * @param day �趨�Ĵ��µĵڼ���[1-31]
        * @param hours �趨�Ĵ���ĵڼ�Сʱ[0-23]
        * @param minutes �趨�Ĵ�Сʱ�ĵڼ�����[0-59]
        * @param seconds �趨�Ĵ˷��ӵĵڼ���[0-59]
        * @return �Ƿ����óɹ�,falseΪû�����óɹ�
        */
        BOOL SetDateTime(UINT32 year, UINT32 month, UINT32 day, UINT32 hours, UINT32 minutes, UINT32 seconds);

        /**
        * @brief ���ñ��������������,ʱ�䲻��
        * @param year ���õ����[1970-2037]
        * @param month �趨�Ĵ�����ĸ���[1-12]
        * @param day �趨�Ĵ��µĵڼ���[1-31]
        * @return �Ƿ����óɹ�,falseΪû�����óɹ�
        */
        BOOL SetDate(UINT32 year, UINT32 month, UINT32 day);

        /**
        * @brief ���ñ����������ʱ��,���ڲ���
        * @param hours �趨�Ĵ���ĵڼ�Сʱ[0-23]
        * @param minutes �趨�Ĵ�Сʱ�ĵڼ�����[0-59]
        * @param seconds �趨�Ĵ˷��ӵĵڼ���[0-59]
        * @return �Ƿ����óɹ�,falseΪû�����óɹ�
        */
        BOOL SetTime(UINT32 hours, UINT32 minutes, UINT32 seconds);

        /**
        * @brief ��ȡ��������������
        * @return ��ȡ�����
        */
        UINT32 GetYear() const;

        /**
        * @brief ��ȡ���������������ʱ�����ڵ���ݵĵڼ�����
        * @return ��ȡ���·�
        */
        UINT32 GetMonth() const;

        /**
        * @brief ��ȡ���������������ʱ�����ڵ��·ݵĵڼ���
        * @return ��ȡ������
        */
        UINT32 GetDay() const;

        /**
        * @brief ��ȡ���������������ʱ�����ڵ���ĵڼ���Сʱ
        * @return ��ȡ��Сʱ��
        */
        UINT32 GetHour() const;

        /**
        * @brief ��ȡ���������������ʱ�����ڵ�Сʱ�ķ�����
        * @return ��ȡ�ķ�����
        */
        UINT32 GetMinute() const;

        /**
        * @brief ���ñ��������������ʱ�����ڵķ��ӵ�����
        * @return ��ȡ������
        */
        UINT32 GetSecond() const;

		/**
        * @brief ���ñ��������������ʱ�����ڵ�������
        * @return ��ȡ��������
		*		0Ϊ������Sunday
		*		1Ϊ����һMonday
		*		... ...
		*		6Ϊ������Saturday
        */
		UINT32 GetWeek() const;

        /**
        * @brief ��SDDateTime��ĳ�ָ�ʽ���и�ʽ����YYYY-mm-dd hh:mm:ss
        * @param pszFormat : ʱ���ʽ
        * @return ��ʽ�����ַ���
        */
        tstring ToString(const TCHAR * pszFormat);

		/**
        * @brief ��SDDateTime��ĳ�ָ�ʽ���и�ʽ��(�޲α�ʾĬ�ϵ�һ�ָ�ʽ)YYYY-mm-dd hh:mm:ss
        * @return ��ʽ�����ַ���
        */
        tstring ToString();

    private:
        /**
        * @brief ���������+
        */
        CRXDateTime & operator + (const CRXDateTime & datetime);

        /**
        * @brief ��������Ƿ������Χ
        * @param year : ���
        * @param month: �·�
        * @param day  : ��
        * @return TRUE:�Ϸ����� FALSE:���Ϸ�����
        */
        BOOL CheckDate(UINT32 year, UINT32 month, UINT32 day);

        /**
        * @brief ���ʱ���Ƿ������Χ
        * @param hours   : Сʱ
        * @param minutes : ����
        * @param seconds : ��
        * @return TRUE:�Ϸ�ʱ�� FALSE:���Ϸ�ʱ��
        */
        BOOL CheckTime(UINT32 hours, UINT32 minutes, UINT32 seconds);

        tm m_time;
    };



    /**
    * @brief �������Ƿ�Ϊ����
    * @param year ������
    * @return ���Ľṹ,TRUEΪ������,FALSE��Ϊ����
    */
    BOOL RXIsLeapYear(UINT32 year);

    /**
    * @brief
    * �����ϵͳ������ĿǰΪֹ��������ʱ�䣨�˺�����Чʱ��Ϊϵͳ������49.7���ڣ�
    * @return �����ϵͳ������ĿǰΪֹ��������ʱ�䣬�����ֵ���ɻ�ã����أ�1
    */
    UINT32  RXGetTickCount();

    /**
    * @brief
    * ����ǰ�̵߳�ִ��ֱ��milliseconds�����
    * @param milliseconds : ָʾ˯�ߵ�ʱ��
    * @return VOID
    */
    VOID  RXSleep(UINT32 milliseconds);

    /**
    * @brief
    * ��ȡ��ǰ��΢��ʱ��
    * @return ��ǰ��΢��ʱ��
    */
    UINT64  RXTimeMicroSec();

    /**
    * @brief
    * ��ȡ��ǰ�ĺ���ʱ��
    * @return ��ǰ�ĺ���ʱ��
    */
    UINT64  RXTimeMilliSec();

    /**
    * @brief
    * ��ȡ��ǰ����ʱ��
    * @return ��ǰ����ʱ��
    */
    UINT64  RXTimeSecs();

    /**
    * @brief ��SDDateTime��ĳ�ָ�ʽ���и�ʽ��(�޲α�ʾĬ�ϵ�һ�ָ�ʽ)
    * @param pszFormat : ʱ���ʽ
    * @param t         : ����ʱ�����
    * @return ��ʽ�����ַ���
    */
    tstring RXTimeToString(const TCHAR* pszFormat,CRXDateTime t);

	/**
    * @brief ��SDDateTime��ĳ�ָ�ʽ���и�ʽ��(�޸�ʽ˵����ʾĬ�ϵ�һ�ָ�ʽ)
    * @param t         : ����ʱ�����
    * @return ��ʽ�����ַ���
    */
    tstring RXTimeToString(CRXDateTime t);

    /**
    * @brief ������ʱ���ַ���ת��ΪCRXDateTime
    * @param strDateTime : ����ʱ���ַ���
    * @param t           : ת�����DateTime����
    * @return TRUE:ת���ɹ� FALSE:ת��ʧ��
    */
    BOOL    RXTimeFromString(tstring strDateTime,CRXDateTime& t);


#endif


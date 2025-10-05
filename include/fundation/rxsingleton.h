#ifndef _RX_SINGLETON_H_
#define _RX_SINGLETON_H_
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif
namespace RX
{
    namespace Fundation
    {

#define SIMPLE_DECLARE_SINGLETON(cls)\
public:\
    static cls* CreateInstance()\
    {\
        static cls oCls;\
        return &oCls;\
    }

#define RX_DECLARE_SINGLETON(cls)\
private:\
	 static cls* m_poInstance;\
public:\
	static BOOL CreateInstance()\
	{\
	if(NULL == m_poInstance)\
	m_poInstance = new cls;\
	return m_poInstance != NULL;\
}\
	static cls* Instance(){ return m_poInstance; }\
	static VOID DestroyInstance()\
	{\
	if(m_poInstance != NULL)\
	{\
	delete m_poInstance;\
	m_poInstance = NULL;\
}\
}

#define RX_IMPLEMENT_SINGLETON(cls) \
	cls* cls::m_poInstance = NULL;


        //The second way to declare a singleton class
#define  SINGLETON_FRIEND_CLASS_REGISTER(T)  friend  class  CSingleton<T>

    /**
    @brief ����ģ����
    *
    *@remarks
    *1.��ĵ�����Ӧ����publicly�ķ�ʽ�̳��Դ��ಢ�ҹ��캯������������Ӧ������Ϊ˽�еġ�
    *2.��������ʹ�õ���������Ӧ�������������캯����operator �� Ϊ˽��
    *3.��Ӻ� SINGLETON_FRIEND_CLASS_REGISTER(T) �������������ע�᱾��Ϊһ��friend��
    */
        template<class  T>
        class   CSingleton
        {
        public:

            /**
            * @brief
            * ��ʼ������������
            * @return false means failed
            */
            static BOOL CreateInstance()
            {
                if (NULL == m_poInstance)
                {
                    m_poInstance = new T;
                }
                return m_poInstance != NULL;
            }

            /**
            * @brief
            * ��ȡ������ָ��
            * @return ������ָ�룬���������ʵ����ʼ��ʧ�ܣ�����NULL
            */
            static T* Instance(VOID)
            {
                return m_poInstance;
            }

            /**
            * @brief
            * ���ٵ�����ʵ��
            * @return VOID
            */
            static VOID DestroyInstance(VOID)
            {
                delete  m_poInstance;
                m_poInstance = NULL;
            }

        protected:
            CSingleton(VOID) {}
            virtual ~CSingleton(VOID) {}

        private:
            static T* m_poInstance;
        };

        template<class T>
        T* CSingleton<T>::m_poInstance = NULL;



        // ����ģʽ����һ��ʵ��,�û�ֻ�趨�� typedef CRXSingleton<Type> TypeSingleton;

        template <class T>
        class CRXSingleton : private T
        {
        private:
            CRXSingleton();
            ~CRXSingleton();

        public:
            static T& Instance();
        };


        template <class T>
        inline CRXSingleton<T>::CRXSingleton()
        {
        }

        template <class T>
        inline CRXSingleton<T>::~CRXSingleton()
        {
        }

        template <class T>
        /*static*/ T& CRXSingleton<T>::Instance()
        {
            // function-local static to force this to work correctly at static
            // initialization time.
            static CRXSingleton<T> s_instance;
            return(s_instance);
        }
    }
}


#endif


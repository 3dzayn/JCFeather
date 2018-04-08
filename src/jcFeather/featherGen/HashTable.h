#pragma once

#ifndef __HashTable
#define __HashTable

#include <vector>
#include <cstddef>

struct Node
{
    void *Key;
	void *Value;

    struct Node *Next;
};
#ifdef LINUX
    #define __stdcall
#endif
//--------------------���ڱȽϵĺ���ָ��
typedef bool (__stdcall *EQUALFUNC)(void *item, void *key);

//--------------------���ڵõ�Hash����ĺ���ָ��
typedef long (__stdcall *GETHASHFUNC)(void *key);


class HashTable
{
    public:
        HashTable(void);
        HashTable(long size);
        HashTable(long size, EQUALFUNC equalFunc, GETHASHFUNC getHashFunc);

    public:
        ~HashTable(void);

    // ��������

        void Add(void *key, void *value);         ///< �ڹ�ϣ�������һ�����(��ڵ�).key����ΪNULL.

        void Remove(void *key);                     ///< �ڹ�ϣ�����Ƴ�һ��ָ�����Ľ��(��ڵ�)

        void Clear();                                       ///< ������н��.

        bool ContainsKey(void *key);              ///< �Ƿ����ָ���ļ�.

        Node* Find(void *key);                        ///< ��ȡָ������Ӧ�Ľ��.�벻Ҫ�޸�Key��Next.

        long Get_Count();                                  ///< �õ���ϣ���н����Ŀ.

		long Get_FreeID();

		long Get_TableLength();

		void Resize(long newSize);                         ///< ���·����С

		void ClearFreePopID();

		void ClearFreeList();

		Node* GetFreeList();

    protected:
		long GetFreeListCount();

        long GetHash(void *key);         ///< ����ָ�����Ĺ�ϣ����.key����ΪNULL.

        bool KeyEquals(void *item, void *key); ///< ��item�ͱ��е�key���бȽ�.

        EQUALFUNC EqualFunc;                      ///< �ȽϺ�����ָ��.���ΪNULL,����DefaultEqualFunc.

        GETHASHFUNC GetHashFunc;             ///< �õ���ϣ����ĺ�����ָ��.���ΪNULL,����DefaultGetHashFunc.

    private:

		long nextFreeID;

        bool inited;                                        ///< �Ƿ��ʼ��.

        long tableLength;                                  ///< ��ϣ��ĳ���

        Node **table;                                     ///< �洢��/ֵ�Եı�.

        long nodeCount;                                   ///< ������Ŀ.

        void init(long size, EQUALFUNC equalFunc, GETHASHFUNC getHashFunc); ///< ��ʼ��.���ڹ��캯��.

        Node *freeList;                                 ///< ����ɾ���Ľ��.������½ڵ�ʱ,�ȴ�����ȡ�����,�������·���.
        void pushFreeList(Node *p);                    ///< ��ӽ�㵽freeList.
        Node *popFreeList();                           ///< ��freeList�е���һ�����,Ϊ���򷵻�NULL.
		long freeListCount;

		std::vector <long> freePopID;					///��������ID�Ŵ洢����Ҫʱ���ȴ�����ȡ��ID
		long popFreeID();								///����һ��ID
		void pushFreeID( void* key );					///�洢һ������ID
	public:
		long maxFreeListCount;
};
#endif

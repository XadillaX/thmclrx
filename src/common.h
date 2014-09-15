/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description:  common functions
 *
 *        Version:  1.0
 *        Created:  2014/09/12 17时45分17秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  XadillaX (http://xcoder.in/), admin@xcoder.in
 *   Organization:  Touhou Gensokyo
 *
 * =====================================================================================
 */
#include <v8.h>
#include <cstring>
#include <node.h>
#include <list>
using namespace std;
using namespace v8;

namespace thmclrx
{
    template <class T>
    class TCX_MemoryPool
    {
    private:
        list<T*>    _pool;

    public:
        ~TCX_MemoryPool()
        {
            Clean();
        }

        T* Create()
        {
            if(_pool.size())
            {
                T* t = _pool.front();
                _pool.pop_front();
                return t;
            }
            else
            {
                return new T();
            }
        }

        void Clean()
        {
            T* t = NULL;
            while(_pool.size())
            {
                t = _pool.front();
                _pool.pop_front();

                delete t;
            }
        }

        void Recycle(T* t)
        {
            memset(t, 0, sizeof(t));
            _pool.push_back(t);
        }
    };

    struct RGB;
    struct ColorCount;

    extern TCX_MemoryPool<RGB> g_PoolRGB;
    extern TCX_MemoryPool<ColorCount> g_PoolColorCount;

    /* RGB struct */
    struct RGB
    {
        unsigned char red;
        unsigned char green;
        unsigned char blue;

        static void recycleArray(RGB* array[], int count)
        {
            for(int i = 0; i < count; i++)
            {
                g_PoolRGB.Recycle(array[i]);
            }
    
            memset(array, 0, count * sizeof(RGB));
        }
    };

    /* Color struct */
    struct ColorCount
    {
        unsigned int count;
        char color[7];

        static bool cmp(ColorCount* a, ColorCount* b)
        {
            if(a->count > b->count) return true;
            if(a->count < b->count) return false;
            return strcmp(a->color, b->color) < 0;
        }
    };
};


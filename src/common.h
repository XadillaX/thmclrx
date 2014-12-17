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
#ifndef __COMMON_H__
#define __COMMON_H__

#include <v8.h>
#include <queue>
#include <cstring>
#include <node.h>
#include <vector>
#include <list>
#include "third-party/xmempool/xmempool.h"
using namespace std;
using namespace v8;

namespace thmclrx
{
    template <class T>
    class TCX_MemoryPool
    {
    private:
        xmem_pool_handle    _pool;

        void _GeneratePool()
        {
            if(!_pool) _pool = xmem_create_pool(sizeof(T));
        }

    public:
        TCX_MemoryPool() :
            _pool(0)
        {
        }

        ~TCX_MemoryPool()
        {
            if(_pool)
            {
                xmem_destroy_pool(_pool);
            }
        }

        inline T* Create()
        {
            _GeneratePool();
            return (T*)xmem_alloc(_pool);
        }

        inline void Clean()
        {
            if(!_pool) return;
            xmem_destroy_pool(_pool);
            _pool = 0;
        }

        inline void Recycle(T* t)
        {
            _GeneratePool();
            xmem_free(_pool, (char*)t);
        }
    };

    struct RGB;
    struct ColorCount;
    struct Palette
    {
        int red;
        int green;
        int blue;
    
        static void GetDefaultPalette(vector<Palette>* palette);
        static void V8ToPalette(Local<Value> value, vector<Palette>* palette);
    };

    extern TCX_MemoryPool<RGB> g_PoolRGB;
    extern TCX_MemoryPool<ColorCount> g_PoolColorCount;
    extern const Palette _rgb8bpp[256];

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

        unsigned char red;
        unsigned char green;
        unsigned char blue;

        unsigned int colorValue;

        static bool cmp(ColorCount* a, ColorCount* b)
        {
            if(a->count > b->count) return true;
            if(a->count < b->count) return false;
            return strcmp(a->color, b->color) < 0;
        }
    };
};

#endif


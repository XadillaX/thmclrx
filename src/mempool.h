/**
 *
 *    ▄▄▄▄▀ ▄  █ █▀▄▀█ ▄█▄    █    █▄▄▄▄     ▄            ,ae,
 * ▀▀▀ █   █   █ █ █ █ █▀ ▀▄  █    █  ▄▀ ▀▄   █          ,88888e
 *     █   ██▀▀█ █ ▄ █ █   ▀  █    █▀▀▌    █ ▀    ,a888b.9888888i
 *    █    █   █ █   █ █▄  ▄▀ ███▄ █  █   ▄ █     888888888888888
 *   ▀        █     █  ▀███▀      ▀  █   █   ▀▄   88888888888888Y
 *           ▀     ▀                ▀     ▀       '8888888888888'
 *                                                  "S888888888"
 *                                                    "7888888Y
 * ThmclrX - A theme color extractor module for Node.js. "e88j
 *                                                         "Y
 * Copyright (c) 2017 XadillaX <i@2333.moe>
 *
 * MIT LIcense <https://github.com/XadillaX/thmclrx/blob/master/LICENSE>
 */
#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__
#include <xmempool/xmempool.h>

namespace __thmclrx__ {

template <class T>
class MemoryPool
{
private:
    xmem_pool_handle    _pool;

    void _GeneratePool()
    {
        if(!_pool) _pool = xmem_create_pool(sizeof(T));
    }

public:
    MemoryPool() : _pool(0) {}
    ~MemoryPool()
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

}

#endif

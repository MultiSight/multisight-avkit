
#include "AVKit/Locky.h"

using namespace AVKit;
using namespace XSDK;
using namespace std;

void Locky::RegisterFFMPEG()
{
    av_register_all();

    av_lockmgr_register( Locky::_LockyCB );
}

void Locky::UnregisterFFMPEG()
{
    av_lockmgr_register( NULL );
}

int Locky::_LockyCB( void** mutex, enum AVLockOp op )
{
    switch( op )
    {
    case AV_LOCK_CREATE:
        *mutex = new XMutex;
        break;
    case AV_LOCK_DESTROY:
        delete ((XMutex*)*mutex);
        break;
    case AV_LOCK_OBTAIN:
        ((XMutex*)(*mutex))->Acquire();
        break;
    case AV_LOCK_RELEASE:
        ((XMutex*)(*mutex))->Release();
        break;
    default:
        break;
    };

    return 0;
}


#include "ext/frameworkext.h"
#include "CMtpServer.h"
#include <elastos/Logger.h>
#include <elastos/Thread.h>
#include "MediaMtp/MtpServer.h"
#include "MediaMtp/MtpStorage.h"
#include <fcntl.h>

using namespace android;
using Elastos::Core::Threading::Thread;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Mtp {

CMtpServer::CMtpServer()
    : mNativeContext(0)
{}

ECode CMtpServer::constructor(
    /* [in] */ IMtpDatabase* database,
    /* [in] */ Boolean usePtp)
{
    NativeSetup(database, usePtp);
    return NOERROR;
}

ECode CMtpServer::Start()
{
    AutoPtr<IThread> thread;
    CThread::New(this, String("MtpServer"), (IThread**)&thread);
    thread->Start();
    return NOERROR;
}

ECode CMtpServer::Run()
{
    NativeRun();
    NativeCleanup();

    return NOERROR;
}

ECode CMtpServer::SendObjectAdded(
    /* [in] */ Int32 handle)
{
    NativeSendObjectAdded(handle);

    return NOERROR;
}

ECode CMtpServer::SendObjectRemoved(
    /* [in] */ Int32 handle)
{
    NativeSendObjectRemoved(handle);

    return NOERROR;
}

ECode CMtpServer::AddStorage(
    /* [in] */ IMtpStorage* storage)
{
    NativeAddStorage(storage);

    return NOERROR;
}

ECode CMtpServer::RemoveStorage(
    /* [in] */ IMtpStorage* storage)
{
    Int32 id;
    storage->GetStorageId(&id);
    NativeRemoveStorage(id);

    return NOERROR;
}

//JNI code
void CMtpServer::NativeSetup(
    /* [in] */ IMtpDatabase* database,
    /* [in] */ Boolean usePtp)
{
    Int32 fd = open("/dev/mtp_usb", O_RDWR);
    if (fd >= 0) {
        Int32 db;
        database->GetNativeContext(&db);
        MtpServer* server = new MtpServer(fd, (MtpDatabase*)db,
            usePtp, 1015/*AID_MEDIA_RW*/, 0664, 0775);
        mNativeContext = (Int32)server;
    }
    else {
        //ALOGE("could not open MTP driver, errno: %d", errno);
    }
}

void CMtpServer::NativeRun()
{
    MtpServer* server = (MtpServer*)mNativeContext;
    if (server)
        server->run();
    else {
        //ALOGE("server is null in run");
    }
}

void CMtpServer::NativeCleanup()
{
    Elastos::Core::Threading::Mutex::Autolock autoLock(mutex);

    MtpServer* server = (MtpServer*)mNativeContext;
    if (server) {
        delete server;
        mNativeContext = 0;
    }
    else {
        //ALOGE("server is null in cleanup");
    }
}

void CMtpServer::NativeSendObjectAdded(
    /* [in] */ Int32 handle)
{
    Elastos::Core::Threading::Mutex::Autolock autoLock(mutex);

    MtpServer* server = (MtpServer*)mNativeContext;
    if (server)
        server->sendObjectAdded(handle);
    else {
        //ALOGE("server is null in send_object_added");
    }
}

void CMtpServer::NativeSendObjectRemoved(
    /* [in] */ Int32 handle)
{
    Elastos::Core::Threading::Mutex::Autolock autoLock(mutex);

    MtpServer* server = (MtpServer*)mNativeContext;
    if (server)
        server->sendObjectRemoved(handle);
    else {
        //ALOGE("server is null in send_object_removed");
    }
}

void CMtpServer::NativeAddStorage(
    /* [in] */ IMtpStorage* storage)
{
    Elastos::Core::Threading::Mutex::Autolock autoLock(mutex);

    MtpServer* server = (MtpServer*)mNativeContext;
    if (server) {
        Int32 storageId;
        storage->GetStorageId(&storageId);
        String path;
        storage->GetPath(&path);
        String description;
        storage->GetDescription(&description);
        Int64 reserveSpace;
        storage->GetReserveSpace(&reserveSpace);
        Boolean removable;
        storage->IsRemovable(&removable);
        Int64 maxFileSize;
        storage->GetMaxFileSize(&maxFileSize);

        const char *pathStr = path.string();
        if (pathStr != NULL) {
            const char *descriptionStr = description.string();
            if (descriptionStr != NULL) {
                MtpStorage* storage = new MtpStorage(storageId, pathStr, descriptionStr,
                        reserveSpace, removable, maxFileSize);
                server->addStorage(storage);
            }
        }
    }
    else {
        //ALOGE("server is null in add_storage");
    }
}

void CMtpServer::NativeRemoveStorage(
    /* [in] */ Int32 storageId)
{
    Elastos::Core::Threading::Mutex::Autolock autoLock(mutex);

    MtpServer* server = (MtpServer*)mNativeContext;
    if (server) {
        MtpStorage* storage = server->getStorage(storageId);
        if (storage) {
            server->removeStorage(storage);
            delete storage;
        }
    }
    else {
        //ALOGE("server is null in remove_storage");
    }
}


} // namespace Mtp
} // namepsace Droid
} // namespace Elastos

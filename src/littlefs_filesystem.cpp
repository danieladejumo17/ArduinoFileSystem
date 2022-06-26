#include "littlefs_filesystem.h"
#include "utilities.h"

// TODO: IGNORE ALL OTHER FILE OPERATIONS IF !_FSReady
LittleFSFileSystem::LittleFSFileSystem() : _FSReady{false}
{
    _FSReady = LittleFS.begin();
    if (!_FSReady)
    {
        //raise Exception
        logfatal("Little FS could not be mounted");
    }
}

LittleFSFileSystem::~LittleFSFileSystem()
{
    LittleFS.end();
}

// ------------------------------------------------
// File Operations
// ------------------------------------------------

File LittleFSFileSystem::open(const String &path, const String &mode)
{
    return LittleFS.open(path.c_str(), mode.c_str());
}

String LittleFSFileSystem::readFile(const String &path)
// call exists before calling this function to verify a file can be read
{
    File file = open(path, "r");
    if (file)
    {
        String content;
        while (file.available())
        {
            content += static_cast<char>(file.read());
        }
        file.close();
        return content;
    }else{
        file.close();
        return "";
    }
}

bool LittleFSFileSystem::writeFile(const String &path, const String &content)
// TDOD: Add functionalitied to write binary data
{
    File file = open(path, "w");
    if (file)
    {
        file.print(content);
        delay(_writeDelay);
        file.close();
        return true;
    }

    return false;
}

bool LittleFSFileSystem::appendToFile(const String &path, const String &content)
{
    File file = open(path, "a");
    if (file)
    {
        file.print(content);
        delay(_writeDelay);
        file.close();
        return true;
    }

    return false;
}

bool LittleFSFileSystem::touch(const String &path)
{
    return writeFile(path, "");
}

bool LittleFSFileSystem::remove(const String &path)
{
    return LittleFS.remove(path);
}

bool LittleFSFileSystem::rename(const String &pathFrom, const String &pathTo)
{
    return LittleFS.rename(pathFrom, pathTo);
}

bool LittleFSFileSystem::copyFile(const String &source, const String &dest)
// TODO: see if there is a better way to do this to work with binary files as well
{
    return writeFile(dest, readFile(source));
}


// ------------------------------------------------
// File and Directory Operations
// ------------------------------------------------

bool LittleFSFileSystem::exists(const String &path)
{
    return LittleFS.exists(path);
}


// ------------------------------------------------
// Directory Operations
// ------------------------------------------------

Dir LittleFSFileSystem::openDir(const String &path)
{
    return LittleFS.openDir(path);
}

bool LittleFSFileSystem::mkdir(const String &path)
{
    return LittleFS.mkdir(path);
}

bool LittleFSFileSystem::rmdir(const String &path)
{
    // remove files in the directory, and remove directories recursively
    Dir dir = openDir(path);
    bool success = true;

    while (dir.next())
    {
        if (dir.isDirectory())
        {
            success = success && rmdir(path + "/" + dir.fileName());
        }
        else
        {
            success = success && remove(path + "/" + dir.fileName());
        }

        if (!success)
        {
            break;
        }
    }

    // when the last file in a subdirectory is removed the subdirectory itself is automatically deleted
    // however in the case where the folder contains no file initially
    if(exists(path))
    {
        success = success && LittleFS.rmdir(path);
    }

    return success;
}

bool LittleFSFileSystem::copyDir(const String &source, const String &dest)
{
    if (validDir(source))
    {
        Dir dir = openDir(source);
        
        if (!exists(dest))
        {
            mkdir(dest);
        }

        while (dir.next())
        {
            String fileName = dir.fileName();
            String sourcePath = source + "/" + fileName;
            String destPath = dest + "/" + fileName;
            if (dir.isDirectory())
            {
                copyDir(sourcePath, destPath);
            }
            else
            {
                copyFile(sourcePath, destPath);
            }
        }
        return true;
    }
    return false;
}

bool LittleFSFileSystem::moveDir(const String &source, const String &dest)
{
    return copyDir(source, dest) && rmdir(source);
}

String LittleFSFileSystem::lsString(const String &path)
{
    String output = "";
    Dir dir = openDir(path);
    while (dir.next())
    {
        output += dir.fileName() + "\n";
    }
    return output;
}
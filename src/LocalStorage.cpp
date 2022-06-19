#include "LocalStorage.h"
#include "utilities.h"

// TODO: IGNORE ALL OTHER FILE OPERATIONS IF !_FSReady
LittleFSLocalStorage::LittleFSLocalStorage() : _FSReady{false}
{
    _FSReady = LittleFS.begin();
    if (!_FSReady)
    {
        //raise Exception
        logfatal("Little FS could not be mounted");
    }
}


// ------------------------------------------------
// File Operations
// ------------------------------------------------

File LittleFSLocalStorage::open(const String &path, const String &mode)
{
    return LittleFS.open(path.c_str(), mode.c_str());
}

String LittleFSLocalStorage::readFile(const String &path)
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

bool LittleFSLocalStorage::writeFile(const String &path, const String &content)
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

bool LittleFSLocalStorage::appendToFile(const String &path, const String &content)
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

bool LittleFSLocalStorage::touch(const String &path)
{
    return writeFile(path, "");
}

bool LittleFSLocalStorage::remove(const String &path)
{
    return LittleFS.remove(path);
}

bool LittleFSLocalStorage::rename(const String &pathFrom, const String &pathTo)
{
    return LittleFS.rename(pathFrom, pathTo);
}

bool LittleFSLocalStorage::copyFile(const String &source, const String &dest)
// TODO: see if there is a better way to do this to work with binary files as well
{
    return writeFile(dest, readFile(source));
}


// ------------------------------------------------
// File and Directory Operations
// ------------------------------------------------

bool LittleFSLocalStorage::exists(const String &path)
{
    return LittleFS.exists(path);
}


// ------------------------------------------------
// Directory Operations
// ------------------------------------------------

Dir LittleFSLocalStorage::openDir(const String &path)
{
    return LittleFS.openDir(path);
}

bool LittleFSLocalStorage::mkdir(const String &path)
{
    return LittleFS.mkdir(path);
}

bool LittleFSLocalStorage::rmdir(const String &path)
{
    return LittleFS.rmdir(path);
}

// ------------------------------------------------

bool LittleFSLocalStorage::copyDir(const String &source, const String &dest)
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

bool LittleFSLocalStorage::moveDir(const String &source, const String &dest)
{
    return copyDir(source, dest) && rmdir(source);
}

char** LittleFSLocalStorage::ls(const String &path)
{
    if (validDir(path))
    {
        Dir dir = openDir(path);
        
        int i = 0;
        while (dir.next())
        {
            i++;
        }
        char** files = new char*[i];
        i = 0;
        dir.rewind();

        while (dir.next())
        {
            files[i] = new char[dir.fileName().length() + 1];
            strcpy(files[i], dir.fileName().c_str());
            i++;
        }
        // dir.close();
        return files;
    }
    return nullptr;
}

String* LittleFSLocalStorage::lsStringArray(const String &path)
{
    char** files = ls(path);

    if (files)
    {
        int i = 0;
        while (files[i])
        {
            i++;
        }
        String* strings = new String[i];
        for (int j = 0; j < i; j++)
        {
            strings[j] = files[j];
        }
        return strings;
    }
    return nullptr;
}

String LittleFSLocalStorage::lsString(const String &path)
{
    String files = "";
    char** filesArray = ls(path);
    if (filesArray)
    {
        int i = 0;
        while (filesArray[i])
        {
            files += filesArray[i];
            files += "\n";
            i++;
        }
        return files;
    }
    return "";
}
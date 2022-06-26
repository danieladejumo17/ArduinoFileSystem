#ifndef LOCAL_STORAGE_HEADER
#define LOCAL_STORAGE_HEADER

#include <Arduino.h>
#include "LittleFS.h"

class LittleFSLocalStorage{
    private:
        bool _FSReady;
        static const int _writeDelay = 100;

        bool validDir(const String &path)
        {
            File file = LittleFS.open(path.c_str(), "r");
            bool isDir = file.isDirectory();
            file.close();
            return isDir;
        }

    public:
        LittleFSLocalStorage();
        ~LittleFSLocalStorage();

        // ------------------------------------------------
        // File Operations
        // ------------------------------------------------

        /**
         * open(path, mode)
         * opens a file in the specified mode and returns a File object
         * */
        File open(const String&, const String&); // returns a File object
        
        /**
         * readFile(path)
         * Reads the content of a text file and returns the content as a String
         * */
        String readFile(const String&);
        
        // TODO: Pass option to prevent overwrite
        /**
         * writeFile(path, content)
         * writes content to the specified file path. Returns true if operation succeeds.
         * overwrites file if file already exists
         * */
        bool writeFile(const String&, const String&); // same as overwrite
        
        /**
         * appendToFile(path, content)
         * appends content to the specified text file. Returns true if operation succeeds.
         * */
        bool appendToFile(const String&, const String&);
        
        /**
         * touch(path)
         * creates an empty text file at the specified path. Returns true if operation succeeds.
         * */
        bool touch(const String&);
        
        /**
         * remove(path)
         * removes the specified file. Returns true if operation succeeds.
         * */
        bool remove(const String&);
        
        /**
         * rename(pathFrom, pathTo)
         * renames the specified file, pathFrom, to pathTo. Returns true if operation succeeds.
         * */
        bool rename(const String&, const String&); //same as moving a file - provid an absolute path
        
        /**
         * copyFile(source, dest)
         * creates a copy of `source` in `dest`. Returns true if operation succeeds.
         * */
        bool copyFile(const String&, const String&);
        bool moveFile(const String &source, const String &dest) { return rename(source, dest); }


        // ------------------------------------------------
        // File and Directory Operations
        // ------------------------------------------------

        /**
         * exists(path)
         * returns true if the specified file or directory exists
         * */
        bool exists(const String&);


        // ------------------------------------------------
        // Directory Operations
        // ------------------------------------------------

        /**
         * openDir(path)
         * opens a directory and returns a Dir object
         * */
        Dir openDir(const String&);

        /**
         * mkdir(path)
         * creates a directory at the specified path. Returns true if operation succeeds.
         * */
        bool mkdir(const String&);

        /**
         * rmdir(path)
         * removes the specified directory. Returns true if operation succeeds.
         * */
        bool rmdir(const String&);

        /**
         * copyDir(source, dest)
         * creates a copy of `source` in `dest`. Returns true if operation succeeds.
         * */
        bool copyDir(const String&, const String&);

        /**
         * moveDir(source, dest)
         * moves `source` to `dest`. Returns true if operation succeeds.
         * */
        bool moveDir(const String&, const String&);
        
        /**
         * lsString(path)
         * returns a list of files and directories in the specified directory as a tab separated String
         * */
        String lsString(const String &path);


        // bool emptyDir(const String &path);
        // bool lenDir(const String &path);
};

static LittleFSLocalStorage localStorage{};

#endif
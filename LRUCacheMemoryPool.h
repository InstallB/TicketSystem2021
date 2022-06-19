//
// Created by Rainy Memory on 2021/3/18.
//

#ifndef TICKETSYSTEM_AUTOMATA_LRUCACHEMEMORYPOOL_H
#define TICKETSYSTEM_AUTOMATA_LRUCACHEMEMORYPOOL_H

#include <iostream>
//#include "HashMap.h"

using std::string;

namespace RainyMemory {
    class MemoryPoolTimeStamp {
    public:
        static int timeStamp;
    };
    
    template <class T, class extraMessage = int>
    class LRUCacheMemoryPool {
        /*
         * class LRUCacheMemoryPool
         * --------------------------------------------------------
         * A class implements memory pool which has built-in cache
         * strategy to storage and quick access data.
         * This class offer single type data's storage, update and
         * delete in file, and LRU Cache to accelerate accession.
         * Also, this class support deleted data's space reclamation,
         * and an extra (not same type of stored data) message storage.
         *
         */
    private:
        enum LogType {
            WRITE, UPDATE, ERASE, UPDATE_EXTRA
        };
        string filename;
        FILE* file;
        
    public:
        explicit LRUCacheMemoryPool(const string &_filename, const extraMessage &ex = extraMessage {}, int _capacity = 100) : filename(_filename){
            file = fopen(filename.c_str(), "rb");
            if (file == nullptr) {
                file = fopen(filename.c_str(), "wb");
                extraMessage temp(ex);
                fwrite(reinterpret_cast<const char *>(&temp), sizeof(extraMessage), 1, file);
            }
            fclose(file);
        }
        
        ~LRUCacheMemoryPool() {}
        
        T read(int offset) {
            file = fopen(filename.c_str(), "rb+");
        	T now;
            fseek(file, offset, SEEK_SET);
            fread(reinterpret_cast<char *>(&now), sizeof(T), 1, file);
            fclose(file);
            return now;
        }
        
        int write(const T &o) {
            file = fopen(filename.c_str(), "rb+");
            fseek(file, 0, SEEK_END);
            fwrite(reinterpret_cast<const char *>(&o), sizeof(T), 1, file);
            int tempWritePoint = ftell(file);
            fclose(file);
            return tempWritePoint;
        }
        
        void update(const T &o, int offset) {
            file = fopen(filename.c_str(), "rb+");
//          T now;
//          fseek(file, offset, SEEK_SET);
//          fread(reinterpret_cast<char *>(&now), sizeof(extraMessage), 1, file);
            fseek(file, offset, SEEK_SET);
            fwrite(reinterpret_cast<const char *>(&o), sizeof(T), 1, file);
            fclose(file);
        }
        
        void erase(int offset) {
//            T temp();
//            file = fopen(filename.c_str(), "rb+");
//            fseek(file, offset, SEEK_SET);
//            fwrite(reinterpret_cast<const char *>(&temp), sizeof(T), 1, file);
//            fclose(file);
        }
        
        void clear(extraMessage ex = extraMessage {}) {
            file = fopen(filename.c_str(), "wb+");
            fclose(file);
            extraMessage temp(ex);
            file = fopen(filename.c_str(), "rb+");
            fseek(file, 0, SEEK_SET);
            fwrite(reinterpret_cast<const char *>(&temp), sizeof(extraMessage), 1, file);
            fclose(file);
        }
        
        extraMessage readExtraMessage() {
            file = fopen(filename.c_str(), "rb+");
            fseek(file, 0, SEEK_SET);
            extraMessage temp;
            fread(reinterpret_cast<char *>(&temp), sizeof(extraMessage), 1, file);
            fclose(file);
            return temp;
        }
        
        void updateExtraMessage(const extraMessage &o, bool doLog = true) {
            file = fopen(filename.c_str(), "rb+");
            extraMessage now;
            fseek(file, 0, SEEK_SET);
            fread(reinterpret_cast<char *>(&now), sizeof(extraMessage), 1, file);
            fseek(file, 0, SEEK_SET);
            fwrite(reinterpret_cast<const char *>(&o), sizeof(extraMessage), 1, file);
            fclose(file);
//            if (doLog) log.logUpdateExtra(MemoryPoolTimeStamp::timeStamp, now);
        }
        
        int tellWritePoint() {
            file = fopen(filename.c_str(), "rb+");
            fseek(file, 0, SEEK_END);
            int tempWritePoint = ftell(file);
            fclose(file);
            return tempWritePoint;
        }
    };
}

#endif //TICKETSYSTEM_AUTOMATA_LRUCACHEMEMORYPOOL_H

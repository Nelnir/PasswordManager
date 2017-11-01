#ifndef ABSTRACTFILEMANAGER_H
#define ABSTRACTFILEMANAGER_H

#include "shared_global.h"
#include <QFile>
#include <QDataStream>
#include <memory>
#include <QDir>
#include <unordered_map>
#include <algorithm>
#include <utility>

template <class T>
class BaseFileManager
{
public:
    BaseFileManager() : m_count(0), m_needsSave(false) {}
    ~BaseFileManager(){ if(m_needsSave){ save(); } }

    bool load(const QString& l_path){
        if(m_cfgFile.isOpen()){
            unload();
        }
        m_cfgFile.setFileName(l_path);
        const QString directory = QFileInfo(l_path).absolutePath();
        if(!QDir(directory).exists()){
            QDir().mkpath(directory);
        }
        if(!m_cfgFile.open(QIODevice::ReadWrite)){
            return false;
        }
        QDataStream stream(&m_cfgFile);
        size_t size = 0;
        stream >> size;
        m_data.reserve(size);
        while(!stream.atEnd()){
            auto acc = std::make_unique<T>();
            stream >> *acc;
            m_data.emplace(m_count++, std::move(acc));
        }
        return true;
    }

    void unload(){
        m_cfgFile.close();
        m_data.clear();
        m_needsSave = false;
    }

    void save(){
        m_needsSave = false;
        if(!m_cfgFile.isOpen()){
            return;
        }
        m_cfgFile.resize(0);
        QDataStream stream(&m_cfgFile);
        stream << m_data.size();
        for(auto& itr : m_data){
            stream << *itr.second;
        }
    }

    bool remove(const int& l_id){
        auto itr = m_data.find(l_id);
        if(itr == m_data.end()){
            return false;
        }
        m_data.erase(itr);
        m_needsSave = true;
        return true;
    }

    int addData(const T& l_acc){
        m_data.emplace(m_count, std::make_unique<T>(l_acc));
        m_needsSave = true;
        return m_count++;
    }

    int addData(std::unique_ptr<T>&& l_ptr){
        m_data.emplace(m_count, std::move(l_ptr));
        m_needsSave = true;
        return m_count++;
    }

    bool replace(const int& l_id, const T& l_acc){
        auto itr = m_data.find(l_id);
        if(itr == m_data.end()){
            return false;
        }
        itr->second = std::make_unique<T>(l_acc);
        m_needsSave = true;
        return true;
    }

    size_t size() { return m_data.size(); }

    const std::unordered_map<int, std::unique_ptr<T>>& container() { return m_data; }

    std::unique_ptr<T>* getItemWithId(const int& l_id){
        auto itr = m_data.find(l_id);
        if(itr == m_data.end()){
            return nullptr;
        }
        return &itr->second;
    }
protected:
    QFile m_cfgFile;
    std::unordered_map<int, std::unique_ptr<T>> m_data;
    int m_count;

    bool m_needsSave;
};

#endif // ABSTRACTFILEMANAGER_H

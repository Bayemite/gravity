#ifndef CIRCLE_BUFFER_H
#define CIRCLE_BUFFER_H

#include <vector>

// Contiguous memory
template <typename T>
class CircleBuffer
{
public:
    CircleBuffer(size_t buffer_size = 256) : m_size{buffer_size},
                                            m_data{}
    {
    }
    
    void push(const T &val)
    {
        if (m_data.size() == m_size)
            m_data.erase(m_data.begin());
        m_data.push_back(val);
    }

    void push(T &&val)
    {
        if (m_data.size() == m_size)
            m_data.erase(m_data.begin());
        m_data.push_back(std::move(val));
    }

    void setSize(size_t size)
    {
        m_size = size;
        if (size_t dataSize = m_data.size(); dataSize > m_size)
        {
            m_data.erase(
                m_data.begin(),
                m_data.begin() + (dataSize - m_size));
        }
    }

    size_t size() const
    {
        return m_data.size();
    }

    T &operator[](size_t index)
    {
        return m_data[index];
    }

    const T& operator[](size_t index) const
    {
        return m_data[index];
    }

    auto begin() const
    {
        return m_data.begin();
    }

    auto end() const
    {
        return m_data.end();
    }

private:
    std::vector<T> m_data;
    size_t m_size;
};

#endif
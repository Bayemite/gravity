#ifndef CIRCLE_BUFFER_H
#define CIRCLE_BUFFER_H

#include <cassert>
#include <vector>

template <typename T>
class CircleBuffer
{
public:
    CircleBuffer(size_t buffer_size = 256) : m_end{0},
                                             m_data{},
                                             m_maxSize{buffer_size}
    {
    }

    void push(const T &val)
    {
        if (m_data.size() < m_maxSize)
        {
            m_data.push_back(val);
        }
        else
        {
            if (m_end == m_data.size())
                m_end = 0;
            m_data[m_end] = val;
        }
        m_end++;
    }

    void push(T &&val)
    {
        if (m_data.size() < m_maxSize)
        {
            m_data.push_back(std::move(val));
        }
        else
        {
            if (m_end == m_data.size())
                m_end = 0;
            m_data[m_end] = std::move(val);
        }
        m_end++;
    }

    void setSize(size_t size)
    {
        if (size > m_data.size())
            m_end = size;
        m_data.resize(size);
        m_maxSize = size;
    }

    size_t size() const
    {
        return m_data.size();
    }

    // Not contiguous! Newest pushed to oldest
    T &operator[](size_t index)
    {
        if (m_end + index >= m_data.size())
            return m_data[m_end - m_data.size() + index];
        return m_data[m_end + index];
    }

    // Not contiguous! Newest pushed to oldest
    const T &operator[](size_t index) const
    {
        if (m_end + index >= m_data.size())
            return m_data[m_end - m_data.size() + index];
        return m_data[m_end + index];
    }

private:
    std::vector<T> m_data;
    size_t m_end;
    size_t m_maxSize;
};

#endif
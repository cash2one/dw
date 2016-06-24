
#include <string>
#include <string.h>
#include <assert.h>
#include "MsgBlock.h"

MsgBlock::MsgBlock()
	: m_data(NULL)
	, m_read_pos(0)
	, m_write_pos(0)
	, m_size(0)
{
	this->doIniting(1);
}

MsgBlock::~MsgBlock()
{
	this->release();
}

int MsgBlock::init(size_t data_len)
{
	this->release();
	return this->doIniting((int)data_len);
}

//int MsgBlock::copy(char * buffer, size_t buf_len)
//{
//	if (this->space() < buf_len)
//	{
//		// fatal error
//		return 0;
//	}
//
//	if (0 == buf_len)
//	{
//		return 0;
//	}
//
//	memcpy(m_data + m_write_pos, buffer, buf_len);
//	m_write_pos += buf_len;
//
//	return buf_len;
//}

void MsgBlock::release()
{
	if (NULL != m_data)
	{
		delete[] m_data;
		m_data = NULL;
	}
}

void MsgBlock::crunch()
{
	if (0 == m_read_pos)
	{
		return;
	}

	if (m_write_pos == m_read_pos)
	{
		m_read_pos = 0;
		m_write_pos = 0;
		return;
	}

	assert(m_read_pos <= m_write_pos);

	memcpy(m_data, m_data + m_read_pos, m_write_pos - m_read_pos);
	m_write_pos = m_write_pos - m_read_pos;
	m_read_pos = 0;
}

char * MsgBlock::rdPtr()
{
	return m_data + m_read_pos;
}

void MsgBlock::rdPtr(size_t read_num)
{
	m_read_pos += read_num;
}

char * MsgBlock::wtPtr()
{
	return m_data + m_write_pos;
}

void MsgBlock::wtPtr(size_t write_num)
{
	m_write_pos += write_num;

	assert(m_write_pos <= m_size);
}

void MsgBlock::clear()
{
	m_read_pos = 0;
	m_write_pos = 0;
}

size_t MsgBlock::size()
{
	assert(m_read_pos <= m_write_pos);

	return m_write_pos - m_read_pos;
}

size_t MsgBlock::space()
{
	assert(m_write_pos <= m_size);
	return m_size - m_write_pos;
}

size_t MsgBlock::capacity()
{
	return m_size;
}

int MsgBlock::doIniting(int data_len)
{
	m_size = data_len;
	m_data = new char[m_size];
	memset(m_data, 0, m_size);
	return 0;
}


#ifndef MSG_BLOCK_HPP__
#define MSG_BLOCK_HPP__

class MsgBlock
{
public:
	MsgBlock();
	~MsgBlock();
public:
	int init(size_t data_len);

	//int copy(char * buffer, size_t buf_len);

	void release();

	void crunch();

	char * rdPtr();

	void rdPtr(size_t read_num);

	char * wtPtr();

	void wtPtr(size_t write_num);

	void clear();

	size_t size();
	size_t space();
	size_t capacity();
protected:
	int doIniting(int data_len);
private:
	char * m_data;
	size_t m_read_pos;
	size_t m_write_pos;
	size_t m_size;
};

#endif

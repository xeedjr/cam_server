/*
 * database.h
 *
 *  Created on: Apr 10, 2014
 *      Author: root
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <sqlite3.h>

class data_base {
	sqlite3 *m_db;
public:
	int open(char *file_dbIn);

	data_base();
	~data_base();
};

#endif /* DATABASE_H_ */

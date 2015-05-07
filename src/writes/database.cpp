/*
 * database.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: root
 */

#include "database.h"

int data_base::open(char *file_dbIn)
{

    if ( file_dbIn == 0 )
    {
        return -1;
    }

    if ( m_db )
    {
        //sqlite3_close( m_db );
        m_db = 0;
    }

    int res = sqlite3_open(file_dbIn, &m_db);
    if ( SQLITE_OK != res )
    {
        return -1;
    }


	return 1;
}

data_base::data_base() {
	// TODO Auto-generated constructor stub

}

data_base::~data_base() {
	// TODO Auto-generated destructor stub
}


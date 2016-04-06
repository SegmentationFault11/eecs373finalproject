#! /bin/bash
mysql --defaults-file=config.cnf < drop_tables.sql
mysql --defaults-file=config.cnf < create_tables.sql
mysql --defaults-file=config.cnf < load_data.sql

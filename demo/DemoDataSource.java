/********************************************************
 * Copyright (c) 2004-2022, Brick Technologies Co., Ltd
 * All rights reserved.
 * Home page:http://www.bricktou.com
********************************************************/
/**
 * This is a MySQL program. 
 * The goal is to convert it to postgresql.
 */
package demo;

import java.sql.Connection;
import java.sql.Driver;
import java.sql.DriverManager; 
import java.sql.PreparedStatement;  
import java.sql.ResultSet;  
import java.sql.ResultSetMetaData;  
import java.sql.SQLException;  
import java.sql.Statement;  
import javax.sql.DataSource;

import com.mysql.cj.jdbc.MysqlDataSource;
import com.mysql.cj.exceptions.CJException;

public class DemoDataSource {
    /**
     * Create DataSource
     */
    public DataSource createDataSource() throws SQLException, CJException {
        try {
            final MysqlDataSource ds = new MysqlDataSource();
            ds.setServerName("localhost");
            ds.setPortNumber(3306);
            ds.setDatabaseName("mydemo");
            ds.setUser("root");
            ds.setPassword("user");
            return ds;
        }
        catch (SQLException sqle) {
            throw new SQLException(sqle.getMessage(), sqle);
        }
        catch (CJException cje) {
            throw new CJException(cje.getMessage(), cje);
        }
    }
    // connection to database
    public Connection getConnDB() throws Exception {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection conn = DriverManager.getConnection(url, user, password);
            return conn;
        }
        catch (Exception e) {
            throw new Exception(e.getMessage(), e);
        }
    }
    private String url = "jdbc:mysql://localhost:3306/demo";
    private String user = "root";
    private String password = "root";
}

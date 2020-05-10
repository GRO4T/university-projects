import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

/**
 * BD - LAB 5,6 example
 *
 * @author Adam Kozlowski <a.kozlowski@ii.pw.edu.pl>
 */
public class JavaAppJDBC {

    private final Connection conn;

    private static final String JDBC_URL = "jdbc:oracle:thin:@ora3.elka.pw.edu.pl:1521:ora3inf";
    private static final String JDBC_USER = "dkolaska";
    private static final String JDBC_PASS = "dkolaska";

    public JavaAppJDBC() throws SQLException {
        this.conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASS);
    }

    /**
     * Using classical statement object
     * @throws SQLException 
     */
    private void doTest1() throws SQLException {

        Statement stmt = conn.createStatement();
        ResultSet rset = stmt.executeQuery("SELECT ename,job FROM emp");

        System.out.println("TEST1 - Query result:");
        int i = 1;
        while (rset.next()) {
            System.out.println("[" + i + "]:" + rset.getString(1));
            System.out.println("[" + i + "]:" + rset.getString(2));
            i++;
        }

        rset.close();
        stmt.close();
    }

    /**
     * Using precompiled statement object
     * @throws SQLException 
     */
    private void doTest2() throws SQLException {

        PreparedStatement stmt = conn.prepareStatement("SELECT ename, job FROM emp");
        ResultSet rset = stmt.executeQuery();

        System.out.println("TEST2 - Query result:");
        int i = 1;
        while (rset.next()) {
            System.out.println("[" + i + "]:" + rset.getString(1));
            System.out.println("[" + i + "]:" + rset.getString(2));
            i++;
        }

        rset.close();
        stmt.close();
    }
    
    public void run() throws SQLException {
        this.doTest1();
        this.doTest2();
    }

    public static void main(String[] args) {
        JavaAppJDBC test;
        try {
            test = new JavaAppJDBC();
            test.run();
        } catch (SQLException ex) {
            System.out.println("Error JavaAppJDBC: " + ex.getMessage());
        }
    }

}

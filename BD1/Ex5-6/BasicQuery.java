import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;

import java.util.Scanner;

public class BasicQuery {

    private final Connection conn;

    private static final String JDBC_URL = "jdbc:oracle:thin:@ora3.elka.pw.edu.pl:1521:ora3inf";
    private static final String JDBC_USER = "dkolaska";
    private static final String JDBC_PASS = "dkolaska";

    public BasicQuery() throws SQLException {
        this.conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASS);
    }

    private void printResults(ResultSet rset) throws SQLException{
        ResultSetMetaData mdata = rset.getMetaData();
        int colCount = mdata.getColumnCount();
        String s = "";
        int i;
        for (i = 1; i <= colCount; i++){
            s += "\t" + mdata.getColumnLabel(i) + "\t";
        }
        System.out.println(s + "\n");

        i = 1;
        while (rset.next()){
            s = "";
            for (int j = 1; j <= colCount; j++){
                s += "\t" + rset.getString(j) + "\t";
            }
            System.out.println("[" + i + "]" + s);
            i++;
        }
    }

    private void doSelect(Scanner scanner) throws SQLException {
        System.out.printf("specjalizacja = ");
        String spec = scanner.nextLine();
        System.out.println("");

        String selectSQL = "SELECT nazwisko, imie FROM lekarz WHERE specjalizacja=?";
        PreparedStatement stmt = conn.prepareStatement(selectSQL);
        stmt.setString(1, spec);
        ResultSet rset = stmt.executeQuery();

        System.out.println("SELECT - Query result:");
        printResults(rset);

        rset.close();
        stmt.close();
    }

    private void doUpdate(Scanner scanner) throws SQLException {
        System.out.printf("stara specjalizacja = ");
        String oldSpec = scanner.nextLine();
        System.out.printf("nowa specjalizacja = ");
        String spec = scanner.nextLine();
        System.out.println("");

        String updateSQL = "UPDATE lekarz SET specjalizacja=? WHERE specjalizacja=?";
        PreparedStatement stmt = conn.prepareStatement(updateSQL);
        stmt.setString(1, spec);
        stmt.setString(2, oldSpec);
        int rowsUpdated = stmt.executeUpdate();

        System.out.println("UPDATE successful: " + rowsUpdated + " rows updated");
        stmt.close();
    }

    private void doDelete(Scanner scanner) throws SQLException {
        System.out.printf("id pacjenta = ");
        int patientID = scanner.nextInt();
        System.out.println("");

        String deleteSQL = "DELETE FROM szczepienie WHERE id_pacjenta=?";
        PreparedStatement stmt = conn.prepareStatement(deleteSQL);
        stmt.setInt(1, patientID);
        int rowsDeleted = stmt.executeUpdate();

        System.out.println("DELETE successful: " + rowsDeleted + " rows deleted");
    }

    public void run(Scanner scanner) throws SQLException {
        this.doSelect(scanner);
        this.doUpdate(scanner);
        this.doDelete(scanner);
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        BasicQuery test;
        try {
            test = new BasicQuery();
            test.run(scanner);
        } catch (SQLException ex) {
            System.out.println("Error JavaAppJDBC: " + ex.getMessage());
        }
        scanner.close();
    }
}

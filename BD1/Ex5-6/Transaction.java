import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Scanner;

public class Transaction {

    private final Connection conn;

    private static final String JDBC_URL = "jdbc:oracle:thin:@ora3.elka.pw.edu.pl:1521:ora3inf";
    private static final String JDBC_USER = "dkolaska";
    private static final String JDBC_PASS = "dkolaska";

    public Transaction() throws SQLException {
        this.conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASS);
    }

    /**
     * pacjent umarl
     * usunac przebyte choroby, recepty, skierowania, szczepienia
     * a potem usunac pacjenta oraz jego dokumenty
     */
    private void doTransaction(Scanner scanner) throws SQLException{
        System.out.printf("id pacjenta = ");
        int patientID = scanner.nextInt();
        System.out.printf("pesel pacjenta = ");
        long patientPESEL = scanner.nextLong();


        PreparedStatement selectPatient = null;
        PreparedStatement deleteDiseaseRecord = null;
        PreparedStatement deleteDoc = null;
        PreparedStatement deletePatient = null;
        PreparedStatement deletePrescription = null;
        PreparedStatement deleteReferral = null;
        PreparedStatement deleteVaccination = null;

        ResultSet rset = null;

        try{
            conn.setAutoCommit(false);
            
            System.out.println("START OF TRANSACTION");

            String docNumber;
            try{
                // uzyskaj numer dokumentu
                selectPatient = conn.prepareStatement(
                    "SELECT numer_dokumentu FROM pacjent WHERE id_pacjenta=? AND pesel=?"
                );
                selectPatient.setInt(1, patientID);
                selectPatient.setLong(2, patientPESEL);
                rset = selectPatient.executeQuery();
                rset.next();
                docNumber = rset.getString(1);
            }
            catch (SQLException e){
                docNumber = "";
            }

            // usun informacje o chorobach
            deleteDiseaseRecord = conn.prepareStatement(
                "DELETE FROM przebyta_choroba WHERE id_pacjenta=? AND pesel_pacjenta=?"
            );
            deleteDiseaseRecord.setInt(1, patientID);
            deleteDiseaseRecord.setLong(2, patientPESEL);
            int deleted = deleteDiseaseRecord.executeUpdate();
            System.out.println(deleted + " rows DELETED from PRZEBYTA_CHOROBA");

            // usun recepty
            deletePrescription = conn.prepareStatement(
                "DELETE FROM recepta WHERE id_pacjenta=? AND pesel_pacjenta=?"
            );
            deletePrescription.setInt(1, patientID);
            deletePrescription.setLong(2, patientPESEL);
            deleted = deletePrescription.executeUpdate();
            System.out.println(deleted + " rows DELETED from RECEPTA");

            // usun skierowania
            deleteReferral = conn.prepareStatement(
                "DELETE FROM skierowanie WHERE id_pacjenta=? AND pesel_pacjenta=?"
            );
            deleteReferral.setInt(1, patientID);
            deleteReferral.setLong(2, patientPESEL);
            deleted = deleteReferral.executeUpdate();
            System.out.println(deleted + " rows DELETED from SKIEROWANIE");

            // usun szczepionki
            deleteVaccination = conn.prepareStatement(
                "DELETE FROM szczepienie WHERE id_pacjenta=? AND pesel_pacjenta=?"
            );
            deleteVaccination.setInt(1, patientID);
            deleteVaccination.setLong(2, patientPESEL);
            deleted = deleteVaccination.executeUpdate();
            System.out.println(deleted + " rows DELETED from SZCZEPIENIE");

            // ostatecznie, usun pacjenta
            deletePatient = conn.prepareStatement(
                "DELETE FROM pacjent WHERE id_pacjenta=? AND pesel=?"
            );
            deletePatient.setInt(1, patientID);
            deletePatient.setLong(2, patientPESEL);
            deleted = deletePatient.executeUpdate();
            if (deleted == 0)
                throw new Exception("No such patient!");
            System.out.println("PATIENT DELETED");

            // jesli pacjent mial taki, usun dokument potwierdzajacy prawo do swiadczen
            if (!docNumber.equals("")){
                deleteDoc = conn.prepareStatement(
                    "DELETE FROM dokument WHERE numer_dokumentu=?"
                );
                deleteDoc.setString(1, docNumber);
                deleteDoc.executeUpdate();
                System.out.println("DOCUMENT DELETED");
            }

            conn.commit();
        }
        catch(SQLException e){
            System.out.println("SQLException: " + e.getMessage());
            System.out.println("Transaction is being rolled back");
            conn.rollback();
        }
        catch (Exception e){
            System.out.println("Exception: " + e.getMessage());
            System.out.println("Transaction is being rolled back");
            conn.rollback();
        }
        finally{
            conn.rollback();

            if (selectPatient != null) selectPatient.close();
            if (deleteDiseaseRecord != null) deleteDiseaseRecord.close();
            if (deleteDoc != null) deleteDoc.close();
            if (deletePatient != null) deletePatient.close();
            if (deletePrescription != null) deletePrescription.close();
            if (deleteReferral != null) deleteReferral.close();
            if (deleteVaccination != null) deleteVaccination.close();

            if (rset != null) rset.close();

            conn.setAutoCommit(true);
            System.out.println("END OF TRANSACTION");
        }
    }

    public void run(Scanner scanner) throws SQLException {
        this.doTransaction(scanner);
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Transaction test;
        try {
            test = new Transaction();
            test.run(scanner);
        } catch (SQLException ex) {
            System.out.println("Error JavaAppJDBC: " + ex.getMessage());
        }
        scanner.close();
    }
}

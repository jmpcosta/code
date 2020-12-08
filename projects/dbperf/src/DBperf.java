// ***********************************************************
//
// Purpose: Test performance of a SQL statement over JDBC
//
// ***********************************************************

/** 
 * @author Joao Costa
 * @version 0.1
*/

// Import packages
import java.sql.*;
import java.time.Duration;
import java.time.LocalTime;
import java.io.FileInputStream;
import java.util.Properties;

/** Database performance class
 *  
 */
class DBperf
{
  // Constant
  public static final String DBPORT = "DATABASE_PORT";

  // Instance scope
  private Properties  props       = null;
  private Connection  conn        = null;
  private String      statement   = null;
  private int         numberTests = 1;
  private int         sleepTime   = 10;
  private int         fetchSize   = 0;
 
/** Single test execution
  * @return Test duration
*/
public Duration test() {

  try (PreparedStatement stmt = conn.prepareStatement( statement, ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY )) {

    stmt.setFetchSize( fetchSize );

    LocalTime startTime = LocalTime.now();
    ResultSet rs        = stmt.executeQuery();
        
    int n = 0;
    while( rs.next() ) {
      n+=1;
    };
        
    LocalTime endTime   = LocalTime.now();

    rs.close();

    System.out.println( "Processed " + n + " rows.");

    return Duration.between( startTime, endTime );
  }
  catch( Exception e ) { 
         System.out.println( e.getStackTrace()[0].getMethodName() + " : " + e );
  }
  
  return Duration.ofSeconds( 0 );
}


/** Execute Test set
*/
public boolean runTests() {

  try {
        System.out.println( "Statement to be executed: " + statement );

        long sum = 0;
        for( int i=0; i < numberTests; i++ )  
           {
             sum += test().toMillis();

             if( i + 1 != numberTests ) {
                 Thread.sleep( Long.valueOf( sleepTime ) * 1000 );
               }
           }

        System.out.println( "Total time is " + sum + " ms with an Average duration of " + sum/numberTests + " ms." );
  }
  catch( Exception e ) { 
      System.out.println( e.getStackTrace()[0].getMethodName() + " : " + e );
  }

  return true;
}


/** Read the provided property file
 *  @param args Array of command line paramenters
 *  @throws Exception
*/
private boolean readPropertyFile( String[] args ) {

  try ( FileInputStream fis = new FileInputStream( args[0] ) ) {

    // create Properties class object
    props = new Properties();

    // load properties file into it
    props.load( fis );

    // Import Properties to instance variables
    statement =                     props.getProperty( "STATEMENT"     );
    fetchSize = Integer.parseInt(   props.getProperty( "FECTH_SIZE"    ) );

    if( args.length >= 2 ) {
        numberTests = Integer.parseInt( args[1] );
    }
    else {
        numberTests = Integer.parseInt( props.getProperty( "NUMBER_TESTS" ) );
    }

    if( args.length >= 3 ) {
        sleepTime = Integer.parseInt( args[2] );
    }
    else {
        sleepTime = Integer.parseInt( props.getProperty( "WAIT_TIME" ) );
    }

  }
  catch( Exception e )  { 
    System.out.println( e.getStackTrace()[0].getMethodName() + " : " + e );
  }

  return true;
}

/** Read connection properties and open the database connection
 *  @throws Exception
*/
private boolean openDatabaseConnection() throws Exception {

  String connectString;
  String username;
  String password;

  connectString = props.getProperty( "DATABASE_PROTOCOL"  );
  connectString += ":";
  connectString += props.getProperty( "DATABASE_TYPE"     );
  connectString += ":";
  
  if( props.getProperty( DBPORT ).length() > 0 )
      connectString += props.getProperty( "DATABASE_HSEP" );

  connectString += props.getProperty( "DATABASE_HOSTNAME" );

  if( props.getProperty( DBPORT ).length() > 0 )  connectString += ":";
  
  connectString += props.getProperty( DBPORT );
  
  connectString += props.getProperty( "DATABASE_SSEP"     );
  connectString += props.getProperty( "DATABASE_SERVICE"  );

  username = props.getProperty( "DATABASE_USERNAME" );
  password = props.getProperty( "DATABASE_PASSWORD" );

  System.out.println( "Using Connection string: " + connectString + " => " + username + "/" + password );

  conn = DriverManager.getConnection( connectString, username, password );

  return true;
}

/** Close Database connection
 * @throws SQLException
 * 
*/
private void closeDatabaseConnection() throws SQLException {
  if( conn != null) conn.close();
}

/** Main
 *  @param args Array of command line paramenters
*/
public static void main( String[] args ) {

  if( args.length == 0 )  {
      System.err.println( "java DBperf <property file name>" );
      return;
    }

  try{
      DBperf dp = new DBperf();

      if( dp.readPropertyFile( args ) && dp.openDatabaseConnection() && dp.runTests() ) {
          dp.closeDatabaseConnection();
      }
        
      
  }
  catch( Exception e ) { 
         System.out.println( e.getStackTrace()[0].getMethodName() + " : " + e );
       }
  }

}   // End of classe scope

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;


public class TCPClient {

	
	
	public static void main(String[] args) {
		try {
			System.out.println("start client...");
			Socket clientSocket = new Socket("127.0.0.1",5100);
			while(true);
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

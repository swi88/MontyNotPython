package monty.remotecontrol;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends Activity {

	private static final String DOMAIN = "10.0.2.2";
	private static final int PORT = 5100;
	
	private Socket socket;
	private static DataInputStream in;
	private static DataOutputStream out;
	private boolean connected = false;
	private ServerListenerThread listener;
	
	private static final String TAKE_PICTURE = "#TAKE_PICTURE";
	private static final String ROTATE_RIGHT = "#ROTATE_RIGHT";
	private static final String ROTATE_LEFT = "#ROTATE_LEFT";
	private static final String FIRST_ARM_UP = "#FIRST_ARM_UP";
	private static final String FIRST_ARM_DOWN = "#FIRST_ARM_DOWN";
	private static final String SECOND_ARM_UP = "#SECOND_ARM_UP";
	private static final String SECOND_ARM_DOWN = "#SECOND_ARM_DOWN";
	
	private static final String QUIT_COMMAND = "#QUIT_COMMAND";
	private static final String GET_IMAGE_COMMAND = "#GET_IMAGE_COMMAND";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// start server listener
		listener = new ServerListenerThread();
		listener.start();
	}
	
	public class ServerListenerThread extends Thread {
				
		public ServerListenerThread() {
		}
		
		@Override
		public void run() {
			// connect to the server
			connect();
			if (connected) {
				String command = "";
				while (!command.equals(QUIT_COMMAND)) {
					try {
						command = in.readUTF();
						if (command.equals(GET_IMAGE_COMMAND)) {
							// Receive Image and write it to the harddrive
							final String fName = in.readUTF();
							byte[] byteArray = new byte[10240];
							int bytesReaded = in.read(byteArray);
							FileOutputStream fos = openFileOutput(fName, Context.MODE_PRIVATE);
							fos.write(byteArray, 0, bytesReaded);
							fos.close();
							runOnUiThread(new Runnable() {
								@Override
								public void run() {
									Intent intent = new Intent(MainActivity.this, ImageActivity.class);
									intent.putExtra("fName", fName);
									startActivity(intent);
								}
							});
						} else if(command.equals(QUIT_COMMAND)) {
							connected = false;
							socket.close();
							System.out.println("CLIENT: quit from server");
							runOnUiThread(new Runnable() {
								@Override
								public void run() {
									Toast.makeText(MainActivity.this, "Server disconnected", Toast.LENGTH_SHORT).show();		
								}
							});
						}
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}
	
	// attempts to connect to chat server and returns true if could, false
	// otherwise
	private boolean connect() {
		try {
			// attempt to connect to server
			socket = new Socket(DOMAIN, PORT);

			// obtain streams
			in = new DataInputStream(socket.getInputStream());
			out = new DataOutputStream(socket.getOutputStream());
			System.out.println("CLIENT: connected to server");
		} catch (UnknownHostException e) {
			Log.e("THREAD", "ERROR: " + e.getMessage());
			System.out.println("ERROR: Unknown Host - " + e.getMessage());
			return false;
		} catch (IOException e) {
			Log.e("THREAD", "ERROR: " + e.getMessage());
			System.out.println("ERROR: IOException - " + e.getMessage());
			return false;
		}
		connected = true;
		return true;
	}
	
	public void onCommand(View v) {
		// send command to server
		if (connected) {
			try {
				String command = null;
				switch(v.getId()) {
				case R.id.buttonTakePicture:
					command = TAKE_PICTURE;
					break;
				case R.id.buttonRotateLeft:
					command = ROTATE_LEFT;
					break;
				case R.id.buttonRotateRight:
					command = ROTATE_RIGHT;
					break;
				case R.id.button1Up:
					command = FIRST_ARM_UP;
					break;
				case R.id.button1Down:
					command = FIRST_ARM_DOWN;
					break;
				case R.id.button2Up:
					command = SECOND_ARM_UP;
					break;
				case R.id.button2Down:
					command = SECOND_ARM_DOWN;
					break;
				}
				// get bytes and send bytes to server
				byte[] utf8Bytes = command.getBytes("UTF8");
				out.writeInt(utf8Bytes.length);
				out.flush();
				out.write(utf8Bytes);
				out.flush();
				System.out.println("CLIENT: sent command to server");
			} catch (IOException e) {
				e.printStackTrace();
				Toast.makeText(this, R.string.server_not_reachable, Toast.LENGTH_SHORT).show();
			}
		} else {
			Toast.makeText(this, R.string.server_not_connected, Toast.LENGTH_SHORT).show();
		}
	}
	
}

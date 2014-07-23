package monty.remotecontrol;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Switch;
import android.widget.Toast;

public class MainActivity extends Activity {

	private static final String DOMAIN = "192.168.42.1"; //Monty: "192.168.42.1"; Lokal: "10.0.2.2";
	private static final int PORT = 5100;
	
	private Socket socket;
	private static BufferedReader inReader;
	private static DataInputStream in;
	private static DataOutputStream out;
	private boolean connected = false;
	private ServerListenerThread listener;
	
	private static final String TAKE_PICTURE = "#TAKE_PICTURE";
	private static final String ROTATE_RIGHT = "#ROTATE_RIGHT";
	private static final String ROTATE_LEFT = "#ROTATE_LEFT";
	private static final String ARM_UP = "#ARM_UP";
	private static final String ARM_DOWN = "#ARM_DOWN";
	
	private static final String ZOOM_IN = "#ZOOM_IN";
	private static final String ZOOM_OUT = "#ZOOM_OUT";
	
	private static final String QUIT_COMMAND = "#QUIT_COMMAND";
	private static final String GET_IMAGE_COMMAND = "#GET_IMAGE_COMMAND";
	private static final String AUTOMATIC_OFF = "#AUTOMATIC_OFF";
	private static final String AUTOMATIC_ON = "#AUTOMATIC_ON";
	
	
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
			if (isInterrupted()) {
				return;
			}
			if (connected) {
//				String command = "";
//				while (!command.equals(QUIT_COMMAND)) {
//					try {
//						// read command
//						StringBuilder builder = new StringBuilder();
//						int c;
//						while ((c = inReader.read()) != -1) {
//							char ch = (char) c;
//							if (ch == '%') {
//								break;
//							}
//						}
//						while ((c = inReader.read()) != -1 && (char) c != '$') {
//							builder.append((char) c);
//						}
//						command = builder.toString();
//						// read the size of the next segment
////						char[] sizeChars = new char[4];
////						ByteBuffer buffer = ByteBuffer.allocate(sizeChars.length);
////						buffer.order(ByteOrder.LITTLE_ENDIAN);
////						in.read(sizeChars);
////						for (int i = 0; i < sizeChars.length; i++) {
////							buffer.put((byte) sizeChars[i]);
////							System.out.println("C" + i + ":" + sizeChars[i] + ", " + buffer.get(i));
////						} 
////						buffer.position(0); 
////						int size = buffer.getInt();
////						System.out.println("size: " + size);
////						char[] commandBytes = new char[size];
////						in.read(commandBytes);
////						command = new String(commandBytes);
//						System.out.println("command: " + command);
//						if (command.equals(GET_IMAGE_COMMAND)) {
//							// read size
//							builder = new StringBuilder();
//							while ((c = inReader.read()) != -1) {
//								char ch = (char) c;
//								if (ch == '%') {
//									break;
//								}
//							}
//							while ((c = inReader.read()) != -1 && (char) c != '$') {
//								builder.append((char) c);
//							}
//							int size = Integer.parseInt(builder.toString());
//							System.out.println("size: " + size);
//							byte[] byteArray = new byte[size];
////							in.read(byteArray); 
//							byteArray[0] = in.readByte();
//							System.out.println("Done!");
//							FileOutputStream fos = openFileOutput("hallo.gif", Context.MODE_PRIVATE);
//							fos.write(byteArray);
//							fos.close();
//							
//							runOnUiThread(new Runnable() {
//								@Override
//								public void run() {
//									Intent intent = new Intent(MainActivity.this, ImageActivity.class);
//									intent.putExtra("fName", "hallo.gif");
//									startActivity(intent);
//								}
//							});
////							// Receive Image and write it to the harddrive
////							final String fName = in.readUTF();
////							byte[] byteArray = new byte[10240];
////							int bytesReaded = in.read(byteArray);
////							FileOutputStream fos = openFileOutput(fName, Context.MODE_PRIVATE);
////							fos.write(byteArray, 0, bytesReaded);
////							fos.close();
//						} else if(command.equals(QUIT_COMMAND)) {
//							connected = false;
//							socket.close();
//							System.out.println("CLIENT: quit from server");
//							runOnUiThread(new Runnable() {
//								@Override
//								public void run() {
//									Toast.makeText(MainActivity.this, "Server disconnected", Toast.LENGTH_SHORT).show();		
//								}
//							});
//						} 
//					} catch (IOException e) {
//						e.printStackTrace();
//					}
//				}
			} else {
				
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
			inReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
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
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(MainActivity.this, "Verbunden mit Monty", Toast.LENGTH_SHORT).show();		
			}
		});
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
					command = ZOOM_IN;
					break;
				case R.id.button1Down:
					command = ZOOM_OUT;
					break;
				case R.id.button2Up:
					command = ARM_UP;
					break;
				case R.id.button2Down:
					command = ARM_DOWN;
					break;
				}
				sendToServer(command);
				System.out.println("CLIENT: sent command to server");
			} catch (IOException e) {
				tryToReconnect();
			}
		} else {
			tryToReconnect();
		}
	}
	
	private void sendToServer(String command) throws IOException {
		// get bytes and send bytes to server
		byte[] utf8Bytes = command.getBytes("UTF8"); 
		System.out.println("bytes count: " + utf8Bytes.length);
		out.writeInt(utf8Bytes.length);
		out.flush();
		out.write(utf8Bytes);
		out.flush(); 
	}

	public void onSwitchClick(View v) {
		if (connected) {
			if (((Switch) v).isChecked()) {
				try {
					sendToServer(AUTOMATIC_OFF);
					automaticMode = false;
				} catch (Exception e) {
				}
				Toast.makeText(this, "Manuelle Steuerung aktiviert", Toast.LENGTH_SHORT).show();
			} else {
				try {
					sendToServer(AUTOMATIC_ON);
					automaticMode = true;
				} catch (Exception e) {
				}
				Toast.makeText(this, "Manuelle Steuerung deaktiviert", Toast.LENGTH_SHORT).show();
			}
		}  else {
			((Switch) v).setChecked(true);
			tryToReconnect();
		}
	}
	
	private void tryToReconnect() {
		Toast.makeText(this, R.string.server_not_connected, Toast.LENGTH_SHORT).show();
		// try to reconnect
		if (!listener.isAlive()) {
			try {
				listener = new ServerListenerThread();
				listener.start();
				Toast.makeText(this, "Verbinde mit Server", Toast.LENGTH_SHORT).show();
			} catch (Exception e) {
			}
		} else {
			Toast.makeText(this, "Verbindungsversuch fehlgeschlagen!", Toast.LENGTH_SHORT).show();
		}
	}
	
	@Override
	protected void onPause() {
		listener.interrupt();
		super.onPause();
	}

	@Override
	protected void onResume() {
		listener = new ServerListenerThread();
		listener.start();
		super.onResume();
	}
	
}

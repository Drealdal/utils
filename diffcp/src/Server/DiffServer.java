package Server;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.HashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class DiffServer {

	private short port = 0x0707;

	public DiffServer() {
	};

	private HashMap<String, HashMap<Integer, byte[]>> digests = new HashMap<String, HashMap<Integer, byte[]>>();
	private HashMap<String, RandomAccessFile> files = new HashMap<String, RandomAccessFile>();
	private ServerSocket server = null;
	private ExecutorService exec = Executors.newCachedThreadPool();

	class receiver implements Runnable {

		Socket socket = null;

		receiver(Socket socket) {
			this.socket = socket;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			try {
				DataInputStream in = new DataInputStream(
						socket.getInputStream());
				DataOutputStream out = new DataOutputStream(
						socket.getOutputStream());
				byte buffer[] = new byte[250];
				byte name[] = new byte[250];

				int len = in.read(name, 0, 250);
				out.writeBoolean(true);

				System.arraycopy(name, 0, buffer, 0, len);
				String filename = new String(name);
				HashMap<Integer, byte[]> map = null;
				synchronized (digests) {
					map = digests.get(filename);
					if (map == null) {
						map = new HashMap<Integer, byte[]>();
						digests.put(filename, map);
					}
				}

				int dlen = MessageDigest.getInstance("MD5").getDigestLength();
				int id = in.readInt();
				byte dig[] = new byte[dlen];
				len = in.read(dig, 0, dlen);
				
				if (dlen != len) {
					System.out.print("Error!" + dlen + ":" + len + "\n");
					return;
				}
				boolean receive = false;
				byte[] digr = null;
				synchronized (map) {
					digr = map.get(id);
					map.put(new Integer(id), dig);
				}
				if (digr == null) {
					receive = true;
				} else {
					receive = false;
					for (int i = 0; i < dig.length; i++) {
						if (dig[i] != digr[i]) {
							receive = true;
						}
					}
				}
				
			
				if (receive) {
					System.out.println("Writing:" + id);
					out.writeBoolean(true);
					long off = in.readLong();
					int ltoread = (int) in.readInt();
					byte data[] = new byte[ltoread];
					int readed = 0;
					while (readed < ltoread) {
						readed += in.read(data, readed, ltoread - readed);
					}
					RandomAccessFile file = null;
					synchronized (files) {
						file = files.get(filename);
						if (file == null) {
							String names[] = filename.split(":");
							file = new RandomAccessFile(names[1], "rw");
							files.put(filename, file);
						}
					}
					synchronized (file) {
						file.seek(off);
						file.write(data);
					}
				} else {
					System.out.println("Ignoring:"+id);
    				out.writeBoolean(false);
				}

			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (NoSuchAlgorithmException e) {
				e.printStackTrace();
			} finally {
				try {
					socket.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

		}

	}

	public void serve() throws IOException {
		server = new ServerSocket(port);
		while (true) {
			Socket socket = server.accept();
			receiver recv = new receiver(socket);
			exec.execute(recv);
			//recv.run();
		}

	}

	/**
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		DiffServer server = new DiffServer();
		System.out.print("Starting server....");
		server.serve();
	}

}

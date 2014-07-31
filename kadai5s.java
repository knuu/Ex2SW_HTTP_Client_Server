import java.net.*;
import java.io.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


class Server {
    public static void main(String[] args) {
	if (args.length < 1) return;
	try {
	    ServerSocket serverS = new ServerSocket(Integer.parseInt(args[0]));
	    while (true) {
		new ServerThread(serverS.accept()).start();
		System.out.println("New connection.");
	    }
	} catch  (IOException e) {
	    System.out.println("IO exception.");
	    System.exit(1);
	}
    }
}

class ServerThread extends Thread {
    Socket clientS;
    PrintWriter toClient;
    String path;
    BufferedReader text;

    public ServerThread(Socket acceptedS) {
	clientS = acceptedS;
    }
    
    public void run() {
	try {
	    toClient = new PrintWriter(clientS.getOutputStream(), true);
	    BufferedReader fromClient = new BufferedReader(new InputStreamReader(clientS.getInputStream()));
	    Pattern p = Pattern.compile("(.+)\\s/(.+)\\sHTTP/1.1");
	    Matcher m = p.matcher(fromClient.readLine());
	    fromClient.readLine();
	    m.find();
	    path = m.group(2);
	} catch (IOException e) {
	    System.out.println("IO exception.");
	    System.exit(1);
	}
	
	boolean NotFound = false;
	try {
	    text = new BufferedReader(new FileReader(path));
	} catch (IOException e) {
	    NotFound = true;
	}
	if (NotFound) {
	    try {	    
		toClient.println("HTTP/1.1 404 Not Fonund\r\nContent-Type: text/html; charset=us-ascii\r\n\r\n<HTML><HEAD>Not Found</HEAD>\r\n<BODY>\r\nThe requested URL /index.html was not found on this server.\r\n</BODY></HTML>\r\n");
		clientS.close();
		return;
	    } catch (IOException e) {
		System.out.println("IO exception.");
		System.exit(1);
	    }		
	}
	try {
	    toClient.print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=iso-2022-jp\r\n\r\n");
	    
	    String content;
	    while ((content = text.readLine()) != null) {
		toClient.println(content);
	    }
	    
	    text.close();
	    clientS.close();
	} catch (IOException e) {
	    System.out.println("IO exception.");
	    System.exit(1);
	}
    }
}
    


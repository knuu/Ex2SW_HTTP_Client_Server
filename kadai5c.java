import java.io.*;
import java.net.*;
import java.util.*;
import java.util.regex.*;

class Client {
    public static void main(String[] args) {
	try {
	    System.out.println("Input URL");
	    System.out.print("> ");
	    BufferedReader din = new BufferedReader(new InputStreamReader(System.in));
	    String input = din.readLine();
	    Browse browser = new Browse(input);
	    while (true) {
		browser.access();
		System.out.println("飛びたいリンクナンバーを入力");
		System.out.println("0を入力でクライアントを終了");
		System.out.print("> ");
		String s = din.readLine();
		int linkNumber = Integer.parseInt(s);
		browser.setLink(linkNumber);
	    }
	} catch (IOException e) {
	    System.out.println("不正な入力1");
	    System.exit(1);
	} catch (NumberFormatException e) {
	    System.out.println("不正な入力2");
	    System.exit(1);
	}
    }
}

class Browse {
    String url,content;
    URL accessURL;
    ArrayList<String> Link;
    BufferedReader in;
    
    public Browse (String input) {
	url = input;
	content = "";
	Link = new ArrayList<String>();
    }

    public void setLink(int linkNumber) {
	if (linkNumber == 0) {
	    System.exit(1);
	}
	try {
	    url = Link.get(linkNumber - 1);
	} catch (IndexOutOfBoundsException e) {
	    System.out.println("不正なリンクナンバー");
	    System.exit(1);
	}
    }

    public void access() {
	try {
	    String line = "";
	    content = "";
	    accessURL = new URL(url);
	    in = new BufferedReader(new InputStreamReader(accessURL.openStream()));
	    while ((line = in.readLine()) != null) {
		System.out.println(line);
		content += line;
	    }
	    in.close();
	    Link.clear();
	    UpdateLinks();
	    ShowLinks();
	} catch (MalformedURLException e) {
	    System.out.println("不正なURL");
	    System.exit(1);
	} catch (UnknownHostException e) {
	    System.out.println("ホストが見つかりません");
	    System.exit(1);
	} catch (IOException e) {
	    System.out.println("HTTP/1.1 404 Not Fonund\r\nContent-Type: text/html; charset=us-ascii\r\n\r\n<HTML><HEAD>Not Found</HEAD>\r\n<BODY>\r\nThe requested URL /index.html was not found on this server.\r\n</BODY></HTML>\r\n");
	    System.exit(1);
	}
 		
    }
    
    private void UpdateLinks() {
	Pattern Linkp = Pattern.compile("<(a\\s(.*?)\\s*href|A\\sHREF)=\"(.+?)\"(.+?)</(a|A)>");
	Matcher Linkm = Linkp.matcher(content);
	while(Linkm.find()) {
	    String temp = Linkm.group(3);
	    Pattern URLp = Pattern.compile("^http://");
	    Matcher URLm = URLp.matcher(temp);
	    if (!URLm.find()) {
		String path = accessURL.getPath();
		temp = "http://" + accessURL.getHost() + path.replaceAll("\\.html", "") + "/" + temp;
	    } 
	    Link.add(temp);
	}
    }
    
    private void ShowLinks() {
	System.out.println("[list of links]");
	for (int i = 0; i < Link.size(); i++) {
	    System.out.println((i + 1) + ". " + Link.get(i));
	}
    }
}

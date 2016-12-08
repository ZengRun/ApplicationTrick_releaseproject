package com;
import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class UpdateXml {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		//String XmlPath = "AndroidManifest.xml";
		
		// TODO Auto-generated method stub
		if (args == null || args.length < 1) {
			System.out.println("No Parameters");
			return;
		}
		if (args[0] == null || args[0].isEmpty() || !args[0].endsWith(".xml")) {
			System.out.println("Wrong Parameters");
			return;
		}
		
		xmlUpdate(args[0]);
//		xmlUpdate(XmlPath);
	}

	public static boolean doc2XmlFile(Document document, String filename) {
		boolean flag = true;
		try {
			
			TransformerFactory tFactory = TransformerFactory.newInstance();
			Transformer transformer = tFactory.newTransformer();
			
			// transformer.setOutputProperty(OutputKeys.ENCODING, "GB2312");
			DOMSource source = new DOMSource(document);
			StreamResult result = new StreamResult(new File(filename));
			transformer.transform(source, result);
		} catch (Exception ex) {
			flag = false;
			ex.printStackTrace();
		}
		return flag;
	}

	public static Document load(String filename) {
		Document document = null;
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			document = builder.parse(new File(filename));
			document.normalize();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		return document;
	}

	public static void xmlUpdate(String xmlPath) {
		Document document = load(xmlPath);
		Node root = document.getDocumentElement();
		String packageName = root.getAttributes().getNamedItem("package")
				.getNodeValue();
		
		System.out
		.println("packageName is "+packageName);
		try {
			if (root.hasChildNodes()) {
				NodeList manifestNodeList = root.getChildNodes();
				
				System.out
				.println("manifestNodeList.getLength() is "+manifestNodeList.getLength());
				
				// manifestNodeList is the list of "manifest" level
				for (int i = 0; i < manifestNodeList.getLength(); i++) {
					// applicationNode is the node of "application"
					Node applicationNode = manifestNodeList.item(i);
					String applicationname ="";
					
					NamedNodeMap applicationAttributeMap = applicationNode.getAttributes();
					if (applicationNode.getNodeName().equals(
									"application")) {
						if(applicationAttributeMap!=null && applicationAttributeMap
								.getNamedItem("android:name") != null){
							applicationname = applicationAttributeMap
									.getNamedItem("android:name")
									.getNodeValue();
							System.out.println("applicationNode's name is : "
									+ applicationname);
							applicationAttributeMap
							.getNamedItem("android:name")
							.setNodeValue(
									"com.cmcc.omss.AppMgr");
						}
						else{
							Element brandElement=(Element)applicationNode;
							brandElement.setAttribute("android:name", "com.cmcc.omss.AppMgr");
						}
						Element xmlElement = document
								.createElement("meta-data");
						
						xmlElement.setAttribute("android:name",
								"APPLICATION_CLASS_NAME");
						
						if (applicationname.startsWith(".")) {
							xmlElement.setAttribute("android:value",
									packageName + applicationname);
						}
						else if(applicationname.length()>0){
							if(applicationname.contains(".")){
								xmlElement.setAttribute("android:value",
										applicationname);
							}
							else{
								xmlElement.setAttribute("android:value",
										packageName + "." + applicationname);
							}
						}
						else {
							xmlElement.setAttribute("android:value",
									"android.app.Application");
						}
						
						applicationNode.appendChild(xmlElement);
					}
				}
				System.out.println("Elements are changing into xml......");
				doc2XmlFile(document, xmlPath);
				System.out.println("AndroidManifest.xml has been done!!!");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
package main;

import java.awt.Color;
import java.awt.Font;
import java.io.FileWriter;
import java.io.IOException;

import edu.princeton.cs.algs4.DijkstraSP;
import edu.princeton.cs.algs4.DijkstraUndirectedSP;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.ST;

/**
 * A class that reads from text files, compiling them into a csv file in which
 * then can be used in EdgeWeightedSymbolGraph for algorithms to parse.
 * 
 * @author Jeffrey Le, Andy Tran
 */

public class Map {
	
    private static ST<String, Location> st;  // string -> warehouse
	
    final static String warehouseConnections = "src/Resources/connections.txt";
	final static String warehouseFile = "src/Resources/map.txt";
	final static String delimiter = ",";
	
	final static Color textColor = Draw.BLACK;
	final static Font textFont = new Font("Arial", Font.BOLD, 14);
	
	final static double warehouseSize = 0.075;
	final static Color warehouseColor = Draw.CYAN;
	
	final static double lineSize = 0.02;
	final static Color lineColor = warehouseColor;
	
	final static int canvasSizeX = 600;
	final static int canvasSizeY = 700;
	
	final static int padding = 100;
	
	public static void buildMap(String[] args) throws IOException {
		st = new ST<String, Location>();
		
		Draw.setCanvasSize(canvasSizeX + padding, canvasSizeY + padding);
		Draw.setFont(textFont);
		
		// create all warehouses
		Draw.setPenRadius(warehouseSize);
        In in = new In(warehouseFile);
        while (in.hasNextLine()) {
        	String[] a = in.readLine().split(delimiter);
        	
            String name = a[0];
            double x = Double.parseDouble(a[1]);
            double y = Double.parseDouble(a[2]);
            
            st.put(name, new Location(name, x, y));
            
            Draw.setPenColor(warehouseColor);
            Draw.point(x/canvasSizeX, y/canvasSizeY);
        }
        
        // create all connections
        FileWriter writer = new FileWriter("src/Resources/CityConnections.csv");
        
        Draw.setPenRadius(lineSize);
        in = new In(warehouseConnections);
        while (in.hasNextLine()) {
        	String[] a = in.readLine().split(",");
        	
        	Location warehouse1 = st.get(a[0]);
        	Location warehouse2 = st.get(a[1]);
        	
        	writer.write(warehouse1.getName() + "," + warehouse2.getName() + "," + warehouse1.distTo(warehouse2) + "\n");
        	
        	Draw.setPenColor(lineColor);
        	Draw.line(warehouse1.getX()/canvasSizeX, warehouse1.getY()/canvasSizeY, warehouse2.getX()/canvasSizeX, warehouse2.getY()/canvasSizeY);

			double averageX = (warehouse1.getX() + warehouse2.getX())/2;
			double averageY = (warehouse1.getY() + warehouse2.getY())/2;
			averageY += 10;
			Draw.setPenColor(Draw.DARK_GRAY);
			Draw.text(averageX/canvasSizeX, averageY/canvasSizeY, String.format("%.0f", warehouse1.distTo(warehouse2)));
        }
        
        writer.close();
        
        // from the new created file, find the shortest path
        EdgeWeightedSymbolDigraph sg = new EdgeWeightedSymbolDigraph("src/Resources/city.csv", ",");
        DijkstraSP mst = new DijkstraSP(sg.digraph(), sg.indexOf("Salt Lake City"));
		
        for (String city : args) {
			Draw.setPenColor(Draw.RED);
			mst.pathTo(sg.indexOf(city)).forEach(edge -> {
				int v = edge.from();
				int w = edge.to();
	
				Location warehouse1 = st.get(sg.nameOf(v));
				Location warehouse2 = st.get(sg.nameOf(w));

				Draw.setPenRadius(0.005);
				Draw.line(warehouse1.getX()/canvasSizeX, warehouse1.getY()/canvasSizeY, warehouse2.getX()/canvasSizeX, warehouse2.getY()/canvasSizeY);
			});
			mst = new DijkstraSP(sg.digraph(), sg.indexOf(city));
		}
        
        st.keys().forEach(key -> {
        	Location l = st.get(key);
        	
        	Draw.setPenColor(textColor);
            Draw.text(l.getX()/canvasSizeX, (l.getY()/canvasSizeY), l.getName());
			
        });
	}
}

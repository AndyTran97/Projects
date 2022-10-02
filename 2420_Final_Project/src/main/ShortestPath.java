package main;

import edu.princeton.cs.algs4.DijkstraSP;
import java.util.ArrayList;

import edu.princeton.cs.algs4.DijkstraUndirectedSP;
import edu.princeton.cs.algs4.DirectedEdge;
import edu.princeton.cs.algs4.Edge;
import edu.princeton.cs.algs4.EdgeWeightedDigraph;
import edu.princeton.cs.algs4.EdgeWeightedGraph;
import edu.princeton.cs.algs4.RedBlackBST;

/**
 * 
 * @author Andy Tran, Jeffrey L
 */
public class ShortestPath {
 
        private double minCost = Double.MAX_VALUE;
        private RedBlackBST<String, ArrayList<ShipItems>> orderST;
        private ArrayList<String> list;
        private String[] shipCity;
        private EdgeWeightedSymbolDigraph sg;
        private String shortestPath;
        
        public ShortestPath(EdgeWeightedSymbolDigraph sg, String[] shipCity, int source){
            this.sg = sg;
            this.shipCity = shipCity;
            int n = shipCity.length;
            minCost = permute(shipCity, 0, n-1, source);
            System.out.println(minCost);
        }

    private double permute(String[] str, int l, int r, int source)
    {   
        if (l == r){
            // start from the source
             minCost = findShortestPath(sg, str, minCost, source);
        }
        else
        {
            for (int i = l; i <= r; i++)
            {
                str = swap(str,l,i);
                permute(str, l+1, r, source);
                str = swap(str,l,i);
            }
        }
        
        return minCost;
    }
 
    private String getString(String[] str){
        String s = "";
        for(int i = 0; i < str.length; i++){
            if(i == str.length - 1){
                s = s + str[i] + ".";    
            }
            else{
                 s = s + str[i] + ", ";
            }

        }
        
        return s;
    }
    
    private double findShortestPath(EdgeWeightedSymbolDigraph sg, String[] str, double min, int s){
        EdgeWeightedDigraph graph = sg.digraph();
        DijkstraSP sp = new DijkstraSP(graph, s);
        
        String path = new String();
        double cost = 0.0;
        int previousV = 0;
        int previousW = 0;
        
        path = path + "Shortest Path From <" + sg.nameOf(s) + "> => " + getString(str) + "\n";
        
        // Find Shortest Path from the source
        for(int i = 0; i < str.length; i++){

            int city = sg.indexOf(str[i]);
            if(sp.hasPathTo(city)){
                for(DirectedEdge e: sp.pathTo(city)){
                    int v = e.from();
                    int w = e.to();
                    String weight = String.format("%.0f", e.weight());

                    path = path + sg.nameOf(v) + " - " + sg.nameOf(w) + " (" + weight + ")\n";                   
                }
                cost = cost + sp.distTo(city);
            }
            sp = new DijkstraSP(graph, city);
        }
        
        // Go back to source
        for(DirectedEdge e: sp.pathTo(s)){
            int v = e.from();
            int w = e.to();
            String weight = String.format("%.0f", e.weight());
            path = path + sg.nameOf(v) + " - " + sg.nameOf(w) + " (" + weight + ")\n";
        }
        
        // Total cost
        cost = cost + sp.distTo(s);
        String costFormat = String.format("%.0f", cost);

        path = path + "_________________________\n";
        path = path + "Total Distance: " + costFormat + "\n\n";
        
        
        // set new shortest path
        if(cost < min){
            shortestPath = path;
            min = cost;
        }
             
        return min;
    }
    
    public String getShortestPath(){           
        return shortestPath;
    }
    

    
    public static String[] swap(String[] a, int i, int j)
    {
        String temp;
        temp = a[i] ;
        a[i] = a[j];
        a[j] = temp;
        return a;
    }           


}

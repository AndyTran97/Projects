package main;

import javax.swing.JFrame;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;

import edu.princeton.cs.algs4.RedBlackBST;  
  
public class BarChart extends JFrame {  
  
  private static final long serialVersionUID = 1L;  
  
 
  
  public BarChart(String appTitle, RedBlackBST<Integer, Items> st) {  
	    super(appTitle);  
	  
	    // Create Dataset  
	    CategoryDataset dataset = createDataset(st);  
	      
	    //Create chart  
	    JFreeChart chart=ChartFactory.createBarChart(  
	        "Bar Chart", //Chart Title  
	        "Products", // Category axis  
	        "Quantity", // Value axis  
	        dataset,  
	        PlotOrientation.VERTICAL,  
	        false,true,false  
	       );  
	  
	    ChartPanel panel=new ChartPanel(chart);  
	    setContentPane(panel);  
	  }
    
  
  private CategoryDataset createDataset(RedBlackBST<Integer, Items> data) {  
    DefaultCategoryDataset dataset = new DefaultCategoryDataset();  
  
    for(Integer k : data.keys()) {
        dataset.addValue(data.get(k).getQuantity(), "In Stock", data.get(k).getName()); 
    }
   
    return dataset;  
  }  
  
  public static void main(String[] args) throws Exception {  

  }  
}  
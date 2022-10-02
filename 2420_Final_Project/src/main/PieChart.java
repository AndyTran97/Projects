package main;

import java.text.DecimalFormat;

import javax.swing.JFrame;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.labels.PieSectionLabelGenerator;
import org.jfree.chart.labels.StandardPieSectionLabelGenerator;
import org.jfree.chart.plot.PiePlot;
import org.jfree.data.general.DefaultPieDataset;
import org.jfree.data.general.PieDataset;

import edu.princeton.cs.algs4.RedBlackBST;  
public class PieChart extends JFrame {  
  private static final long serialVersionUID = 6294689542092367723L;  
  
  
  public PieChart(String title, RedBlackBST<Integer, Items> data) {  
	    super(title);  
	  
	    // Create dataset  
	    PieDataset dataset = createDataset(data);  
	  
	    // Create chart  
	    JFreeChart chart = ChartFactory.createPieChart(  
	        "Pie Chart",  
	        dataset,  
	        true,   
	        true,  
	        false);  
	  
	    //Format Label  
	    PieSectionLabelGenerator labelGenerator = new StandardPieSectionLabelGenerator(  
	        "{0} ({2})", new DecimalFormat("0"), new DecimalFormat("0%"));  
	    ((PiePlot) chart.getPlot()).setLabelGenerator(labelGenerator);  
	      
	    // Create Panel  
	    ChartPanel panel = new ChartPanel(chart);  
	    setContentPane(panel);  
  }  
    
  
  private PieDataset createDataset(RedBlackBST<Integer, Items> data) {  
	  
	    DefaultPieDataset dataset=new DefaultPieDataset();
	    for(Integer k : data.keys()) {
	        dataset.setValue(data.get(k).getName(), data.get(k).getQuantity()); 
	    }

	    return dataset;  
  }
  
  public static void main(String[] args) {  
      
  }  
}  
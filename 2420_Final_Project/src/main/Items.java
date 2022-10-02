package main;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * A class that stores various information about a specific item.
 * Each item is given a unique id that increments with every new item entry.
 * 
 * @author Jeffrey L, Andy T
 *
 */
public class Items implements Comparable<Items>{

	public static int count = 1;
	private int id;
	
	private String code;
	private String name;
	private double price;
	private int quantity;	
	private Date dateCreated;	
	private String department;
	
	/**
	 * Constructor of Items.
	 * 
	 * @param code			String	code given to item
	 * @param name			String	name of item
	 * @param price			double	price of item
	 * @param quantity		int		quantity of item
	 * @param dateCreated	Date	date item was added to database
	 * @param department	String	department handling the item
	 */
	public Items(String code, String name, double price, int quantity, Date dateCreated, String department) {
		super();
		this.id = count++;
		this.code = code;
		this.name = name;
		this.price = price;
		this.quantity = quantity;
		this.dateCreated = dateCreated;
		this.department = department;
	}

	/**
	 * Returns the id of the item.
	 * 
	 * @return	int	the id
	 */
	public int getId() {
		return id;
	}
	
	/**
	 * Returns the code of the item.
	 * 
	 * @return	String	the code
	 */
	public String getCode() {
		return code;
	}
	
	/**
	 * Sets the code of the item.
	 * 
	 * @param code	String	the new code
	 */
	public void setCode(String code) {
		this.code = code;
	}
	
	/**
	 * Returns the name of the item.
	 * 
	 * @return	String	the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * Sets the name of the item.
	 * 
	 * @param name	String	the new name
	 */
	public void setName(String name) {
		this.name = name;
	}
	
	/**
	 * Returns the price of the item.
	 * 
	 * @return	double	the price
	 */
	public double getPrice() {
		return price;
	}

	/**
	 * Sets the price of the item.
	 * 
	 * @param price	double	the new price
	 */
	public void setPrice(double price) {
		this.price = price;
	}
	
	/**
	 * Returns the quantity of the item.
	 * 
	 * @return	int	the quantity
	 */
	public int getQuantity() {
		return quantity;
	}

	/**
	 * Sets the quantity of the item.
	 * 
	 * @param quantity	int	the new quantity
	 */
	public void setQuantity(int quantity) {
		this.quantity = quantity;
	}

	/**
	 * Returns the date created of the item.
	 * 
	 * @return	String	the date created
	 */
	public String getDateCreated() {
   		String pattern = "MM/dd/yyyy";
   		SimpleDateFormat simpleDateFormat = new SimpleDateFormat(pattern);
		return simpleDateFormat.format(dateCreated);
	}

	/**
	 * Returns the department of the item.
	 * 
	 * @return	String	the department
	 */
	public String getDepartment() {
		return department;
	}

	/**
	 * Sets the department of the item.
	 * 
	 * @param department	String	the new department
	 */
	public void setDepartment(String department) {
		this.department = department;
	}

	/**
	 * Returns a String in the following format, describing the item:
	 * 
	 * 
	 * @return	String	item info
	 */
	@Override
	public String toString() {
            return id + ". " + name + ", " + code + ", " + String.format("%.2f",price) + ", " + quantity + ", " + department + ", " + this.getDateCreated() + ".";
	}
	
	
	/**
	 * Returns a String in the following format, describing the item:
	 * 
	 * 
	 * @return	String	item info
	 */
	public String getString() {
		return id + "," + name + "," + code + "," + String.format("%.2f",price) + "," + quantity + "," + department + "," + this.getDateCreated();
	}
        
	public static void main(String[] args) {
//		Items i1 = new Items("AB34", "Bob Ross", 3.14, 1, new Date(1, 1, 2022), "Computer");
//		Items i2 = new Items("AB34", "Bob Ross", 3.14, 1, new Date(1, 1, 2022), "Computer");
//		Items i3 = new Items("AB34", "Bob Ross", 3.14, 1, new Date(1, 1, 2022), "Computer");
//
//		StdOut.println(i1.getId());
//		StdOut.println(i2.getId());
//		StdOut.println(i3.getId());
	}

	@Override
	public int compareTo(Items other) {
		return Integer.compare(id, other.getId());
	}

}
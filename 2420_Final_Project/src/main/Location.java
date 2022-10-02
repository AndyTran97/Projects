package main;

/**
 * A class that describes a location's name and x, y position.
 * 
 * @author Jeffrey Le, Andy Tran
 *
 */
public class Location {

	private String name;
	
	private double x;
	private double y;
	
	/**
	 * Constructor of Location.
	 * 
	 * @param name	String	the name
	 * @param x		double	the x location
	 * @param y		double	the y location
	 */
	public Location(String name, double x, double y) {
		this.name = name;
		this.x = x;
		this.y = y;
	}

	/**
	 * Gets the distance between two locations given the
	 * other location.
	 * 
	 * @param other	Location	the other location
	 * @return		double		the distance
	 */
	public double distTo(Location other) {
		double x = this.x - other.x;
		double y = this.y - other.y;
		
		x *= x;
		y *= y;
		
		return Math.sqrt(x + y);
	}

	/**
	 * Returns the name of the location.
	 * @return	String	the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * Returns the x position of the location.
	 * @return	double	the x position
	 */
	public double getX() {
		return x;
	}

	/**
	 * Returns the y position of the location.
	 * @return	double	the y position
	 */
	public double getY() {
		return y;
	}
	
}

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package main;

/**
 *
 * @author Andy Tran, Jeffrey Le
 */
public class ShipItems
{
    private int id;
    private int quantity;
    
    public ShipItems(int id, int quantity){
        this.id = id;
        this.quantity = quantity;
    }
    
    public int getID(){
        return id;
    }
    
    public int getQuantity(){
        return quantity;
    }
    
    @Override
    public String toString(){
        return "[id=" + id + ",quantity=" +quantity + "]";
    }
    
    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }

        if (obj.getClass() != this.getClass()) {
            return false;
        }

        final ShipItems other = (ShipItems) obj;
        if(this.id == other.getID() && this.quantity == other.getQuantity()){
            return true;
        }

        return false;
    }
}

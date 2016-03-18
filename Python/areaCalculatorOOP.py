class Rectangle(object):
    
    def __init__(self, height, width):
        self.height = height
        self.width = width
        print self.area()
        
    def area(self):
        return self.height * self.width
        
def main():
    height = int(raw_input("Enter height: "))
    width = int(raw_input("Enter width: "))
    
    rect = Rectangle(height, width)
    
    rect.height = height;
    rect.width = width;
    
if __name__ == "__main__":
    main()
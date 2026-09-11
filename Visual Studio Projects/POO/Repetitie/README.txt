This project includes:
- virtual functions behavior

virtual function:
    the base class creates a virtual table for function implementations references;
    while upcasting, the base class pointer searches the virtual table for a virtual function's latest available implementation


- presentation of overriding the read/write functions 

derived class function first calls the base class function, then makes the necessary actions for its extended data.
the operators >> and << are written for base class arguments only, but may also take derived class arguments and call their corresponding read/write function.


- presentation of a way to extract variable name
#define getName(x) #x
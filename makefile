JC = javac
JVM = java 
SRC = src/main/java/com/elo321/
.SUFFIXES: .java .class

CLASSES = Person.java Bridge.java Main.java

classes:
	$(JC) $(SRC)*.java

run: classes
	$(JVM) -cp src/main/java com.elo321.Main

clean:
	$(RM) $(SRC)*.class

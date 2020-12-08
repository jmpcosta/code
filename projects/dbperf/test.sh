#!/bin/sh

# Test the performance of an SQL statement w5 times with a delay of 10 seconds
java -classpath /usr/share/java/mysql-connector-java-8.0.22.jar:. DBperf /home/joao/workspace/code/projects/dbperf/run.properties 5 10

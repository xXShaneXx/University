all:
	go build -o mutex_template mutex_template.go rw_monitor.go monitor_package.go

run:
	./mutex_template > out1
	bash ../display-travel-2.bash out1
clean:
	rm ./mutex_template
	rm ./out1
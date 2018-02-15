package hello;

import java.util.concurrent.atomic.AtomicLong;

import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class GreetingController {

    private static final String template = "Hello, %s!";
    private final AtomicLong counter = new AtomicLong();
    private Sensors sensors = new Sensors();
    

    @RequestMapping("/greeting")
    public Greeting greeting(@RequestParam(value="name", defaultValue="World") String name) {
        return new Greeting(counter.incrementAndGet(),
                            String.format(template, name));
    }

    @RequestMapping("/sensors/temperature")
    public int getTemperature(@RequestParam(value="name", defaultValue="temperature") String name) {
    	return sensors.getTemperature();
    }

    @RequestMapping("/sensors/temperature/{temperature}")
	public void setTemperature(@PathVariable int temperature) {
    	sensors.setTemperature(temperature);    	
    }


}

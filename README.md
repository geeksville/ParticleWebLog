# ParticleWebLog

A Particle library for remote logging via publish().  And example config
for loggly.com.

I wrote this tiny logging framework because I wanted something that would allow
remote logging but use **very** little FLASH space.

Other benefits:

* Messages are not sent in cleartext (they are inside the particle encrypted link).
* Because this library doesn't use UDP, it is probably a more resistant to carrier
throttling on cellphone networks (i.e. for the Electron)

## Usage

```
#include "ParticleWebLog.h"
ParticleWebLog particleWebLog;

void setup() {
  Log.info("Hi I'm a log message");
}

void loop() {
}
```

See the [examples](examples) folder for more details.

## Documentation

This library merely registers a log provider that publishes each logged string
as a particle publish event.  The events will be published using the name of
your choice (defaults to "log").

Limitations:

* This tiny library is built on top of Particle.publish, so you should not
print log message too quickly if you are using it.  It might drop messages if
you send more than about 1 a second.
* Third generation particle devices have lots of log messages from the system
some of which seem to come out before publishing is legal.  So this log provider
only logs "app" messages.

## Using web logging services

One of the great things about this approach is that the Particle.io web service
has good webhook support.  So you can squirt these crude log messages to a nice storage/viewer
service.  Most any service that has a way to accept HTTP posts of log messages should work.
Here is an example using loggly.com:

* Go to loggly.com and create a free account (you can pick any domain name).
* Go to the console for your domain: https://yourdomain.loggly.com/sources/setup/https
and setup an HTTPS log source.  It will give you an URL that looks approximately
like this: https://logs-01.loggly.com/inputs/XXXXXXXX-76a8-4af5-a967-XXXXXXXXXXXX/tag/http/.
* Go to the Particle.io web console and click on "Integrations / New Integration / Webhook".
* On the form it shows, enter "log" for Event Name, the URL you were provided above, change the request format to
JSON.
* Then click to enter custom JSON data and paste in the following:
```
{
  "message": "{{{PARTICLE_EVENT_VALUE}}}",
  "from": "{{{PARTICLE_DEVICE_ID}}}",
  "published_at": "{{{PARTICLE_PUBLISHED_AT}}}",
  "userid": "{{{PRODUCT_USER_ID}}}",
  "fw_version": "{{{PRODUCT_VERSION}}}"
}
```
* Click "save" to save your new integration.
* In the top right of the integration you should now see a "Test" button.  You can
click it to test that the Particle server is now able to talk with Loggly.
* Any new log publishes from your device should now be stored in Loggly.

## Contributing

I will happily accept pull requests and respond to issues raised in github.

Thanks to @barakewi for his [Papertrail library](https://github.com/barakwei/ParticlePapertrail) which was used
as a template to create this library.

## LICENSE
Copyright 2019 kevinh@geeksville.com

Licensed under the <insert your choice of license here> license

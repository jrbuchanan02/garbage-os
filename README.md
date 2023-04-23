# Garbage OS
Garbage OS is an open-source operating system that:
1. Does not take itself seriously
2. Intends to educate on the functionality of operating systems by providing a 
readable and functioning example of one.
3. Provides (or will provide) the ability to create a High Performance Computing
 Cluster (HPC Cluster) at the Operating System level.

Garbage OS is published under the LGPL version 2.1. Garbage OS is open source: 
you are free to view, build, modify, and make projects from Garbage OS.
Additionally, LGPL allows closed-source projects to rely on Garbage OS, in
addition to open source projects.

## Terms of Use
As expressly stated in the license, Garbage OS comes as-is and with no warranty.
Additionally, Garbage OS ships with the terms that all responsibility (good and 
bad) that comes from whatever Garbage OS does (or commands the computer to do)
is shared with (1) whomever installed Garbage OS on the system, (2) whomever
owns the computer, (3) the person who is using the computer or made the decision
to leave the computer unattended. Another way of thinking of these terms is that
Garbage OS is "use at your own risk". 

Let's give an example.
Imagine that John Doe installed Garbage OS on Joe Schmoe's computer and Billy 
Bob left the computer unattended to get coffee. If the unattended computer ate
Sally's kitten, John Doe, Joe Schmoe and Billy Bob would all be responsible, 
similarly, if the computer stopped an alien invasion set to destroy humanity, 
John Doe, Joe Schmoe, and Billy Bob would have a genuine claim to saving 
humanity from certain destruction.

- this hypothetical does not change if any of the people in the example are 
entities (eg, companies, governments)
- this hypothetical does not change if any of the people in the example are 
the same person (eg, the computer's owner installs Garbage OS)
- all actions taken by Garbage OS, intended by the developers or not, apply to 
this hypothetical. Even in circumstances where the behavior Garbage OS exhibits
is not intended by the developers (eg, a "bug" or a "glitch"), the people 
responsible do not know about the features, or if the "thing Garbage OS does" is
failing to take action (that is, an inaction taken by Garbage OS follows the 
same rules as any action taken by Garbage OS).

These Rules, however, would not apply if the action taken by Garbage OS was a 
message to the user from the developers. For example, a "rogue" developer places
some objectionable comment, term, statement, etc. to send to the user. In this
instance, that specific "rogue" developer would be responsible.

## Privacy

Garbage OS currently does not collect any data on users. Even if data were collected
by any user, there would be no location to place said datta. However, if Garbage
OS eventually collects user data, this data would be designed for the sole purpose
of debugging Garbage OS and any interaction that the operating system has with 
the specific type of hardware. To put it bluntly, we don't care about your personal
info, we're interested in the computer.

Examples of information that Garbage OS might at some point collect:
1. Your computer's firmware / motherboard revision. This information would be 
necessary to determine if any debugging-worthy event is caused by Garbage OS, 
the specific computer, or a broader issue with all computers like the one that
caused the event.
2. Your computer's specific hardware information. This information includes items
such as your computer's MAC address, an identifier unique to each individual network
card / interface (so, your cellphone might have one for the cellular network and 
one for Wi-Fi). Your computer's serial number. Your processor's serial number, 
etc. This information would be necessary for identifying very specific and 
obscure sets of behavior.
3. The exact state of the processors' hardware when the event occurs. This 
information only includes the memory of the kernel and the specific program that
causes the event. So, if you are running a web browser and the file explorer 
causes the event, the web browser's information is not included. This information
is necessary to parse the code of Garbage OS in extreme detail and more quickly
resolve the issue.

Will you be able to opt-out of reporting: yes.

What will not be collected: personal identifying information. Until Garbage OS
can remove this personal information from reports first-time every-time, you will
be able to see all of the information that Garbage OS intends to report before
the report is sent. You can personally remove this information. The report will
note that the information was changed, but will not include any method to 
reconstruct changed data. That is, the information you remove will not be 
reported. 

Should Garbage OS become 100% accurate at removing such personal 
information, personal identifying information will include names (partial, full,
nicknames, even usernames), contact information (emails, addresses, phone numbers),
financial, banking, and government-identification (account numbers, social security
numbers and international equivalents, drivers license numbers, etc.) and 
"pinpointing information" (ip-addresses, location data, and, in some circumstances
photos, videos, and the state of connected hardware, should this information be
used to "pinpoint" your location or tie your location to a specific time), 
"what you were doing information" (information about the state of the computer
that reporting would be less about finding the issue and more about snooping 
around, such as the state of applications not involved in the error, states of 
part of the application that did not actively cause the error, information 
recently provided to the computer that did not go to the application causing 
the error, etc.).

In other words, the information that would be provided in a report that Garbage
OS would automatically screen is thus:
- the current state of the processor and any co-processors (such as a graphics
card) involved in the program that causes the error. If, for example, the program
is only using the CPU, only the CPU would be present in the report.
- the state of memory for the part of the program that causes the error. For 
example, a specific browser tab, if such isolation is possible. 
- the state of the Garbage OS kernel
- the state of devices involved in the error, minus any information that can be
used in a manner to locate the computer or anyone using it.
- the time and date of the error along with the exact version of Garbage OS, the
exact revision of system drivers, and the revision of the system as a whole.

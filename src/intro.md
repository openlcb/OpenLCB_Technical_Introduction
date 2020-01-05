# Introduction

Let's start with what to some might be confusing--the name LCC vs OpenLCB.
LCC is a brand name owned by the NMRA. It covers a set of standards that have
been adopted by the NMRA for Layout Command Control (hence the LCC).

OpenLCB is the name of the working group that created the standards approved by
the NMRA and continues to create new standards. This is a group of dedicated
volunteers who are working to fulfill the dream.

In this book, as in the standards, we'll use the term OpenLCB rather than LCC.

## Purpose

The purpose of this book is to help you get up to speed on OpenLCB so you can
start creating LCC products. There are a lot of concepts and we'll go into
just enough depth to get you going, without bogging you down with all the
details and possibilites. The standards and the techincal notes have a lot
more details.

## Focus of this book

We're going to focus on how to create _node_ that can be added to an
LCC bus. Therefore, we're going to focus on solutions that use the CAN
bus. There are other options, such as WiFi, but we won't cover them in
this book.

## Assumptions

We're assuming you've used LCC products, and therefore already understand
concepts like the producer-consumer model. We also assume you've written
code for micro controllers and have worked with I/O pins.
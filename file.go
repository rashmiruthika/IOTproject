package main

import (
	"encoding/json"
	"fmt"
	"log"
	"math/rand"
	"net/http"
	"sync"
	"time"
)

// TemperatureData represents the temperature data structure
type TemperatureData struct {
	Temperature float64 `json:"temperature"`
	Timestamp   string  `json:"timestamp"`
}

// Global variable to store the temperature data
var (
	tempData = TemperatureData{}
	mu       sync.RWMutex
)

// simulateTemperatureReading simulates reading temperature data from a sensor
func simulateTemperatureReading() {
	for {
		time.Sleep(5 * time.Second) // Simulate a sensor reading every 5 seconds

		// Lock the mutex to update the temperature data safely
		mu.Lock()
		tempData = TemperatureData{
			Temperature: 25.0 + (rand.Float64() * 10.0), // Simulate a temperature between 25°C and 35°C
			Timestamp:   time.Now().Format(time.RFC3339),
		}
		mu.Unlock()
	}
}

// handleTemperature returns the latest temperature data
func handleTemperature(w http.ResponseWriter, r *http.Request) {
	// Lock the mutex to read the temperature data safely
	mu.RLock()
	data := tempData
	mu.RUnlock()

	w.Header().Set("Content-Type", "application/json")
	if err := json.NewEncoder(w).Encode(data); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func main() {
	// Start a goroutine to simulate temperature readings
	go simulateTemperatureReading()

	// Set up HTTP handlers
	http.HandleFunc("/temperature", handleTemperature)

	// Start the server
	addr := ":8089"
	fmt.Printf("Starting server at %s\n", addr)
	if err := http.ListenAndServe(addr, nil); err != nil {
		log.Fatal(err)
	}
}

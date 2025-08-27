<?php
require_once __DIR__ . '/config.php';

enable_cors();

try {
    $pdo = get_pdo();
} catch (Throwable $e) {
    json_response(['error' => 'Database connection failed', 'details' => $e->getMessage()], 500);
}

$method = $_SERVER['REQUEST_METHOD'] ?? 'GET';

switch ($method) {
    case 'GET':
        handle_get($pdo);
        break;
    case 'POST':
        handle_post($pdo);
        break;
    case 'PUT':
        handle_put($pdo);
        break;
    case 'DELETE':
        handle_delete($pdo);
        break;
    default:
        json_response(['error' => 'Method not allowed'], 405);
}

function handle_get(PDO $pdo): void {
    $stmt = $pdo->query('SELECT id, title, is_completed, created_at, updated_at FROM tasks ORDER BY id DESC');
    $rows = $stmt->fetchAll();
    foreach ($rows as &$row) {
        $row['is_completed'] = (bool)$row['is_completed'];
    }
    json_response(['tasks' => $rows]);
}

function get_json_input(): array {
    $raw = file_get_contents('php://input');
    if ($raw === false || $raw === '') {
        return [];
    }
    $data = json_decode($raw, true);
    if (!is_array($data)) {
        json_response(['error' => 'Invalid JSON body'], 400);
    }
    return $data;
}

function handle_post(PDO $pdo): void {
    $data = get_json_input();
    $title = trim((string)($data['title'] ?? ''));
    if ($title === '') {
        json_response(['error' => 'Title is required'], 422);
    }
    $stmt = $pdo->prepare('INSERT INTO tasks (title, is_completed, created_at, updated_at) VALUES (?, 0, NOW(), NOW())');
    $stmt->execute([$title]);
    $id = (int)$pdo->lastInsertId();
    $task = fetch_task($pdo, $id);
    json_response(['task' => $task], 201);
}

function handle_put(PDO $pdo): void {
    $data = get_json_input();
    $id = isset($data['id']) ? (int)$data['id'] : 0;
    if ($id <= 0) {
        json_response(['error' => 'Valid id is required'], 422);
    }
    $fields = [];
    $params = [];
    if (array_key_exists('title', $data)) {
        $title = trim((string)$data['title']);
        if ($title === '') json_response(['error' => 'Title cannot be empty'], 422);
        $fields[] = 'title = ?';
        $params[] = $title;
    }
    if (array_key_exists('is_completed', $data)) {
        $isCompleted = (int)!!$data['is_completed'];
        $fields[] = 'is_completed = ?';
        $params[] = $isCompleted;
    }
    if (empty($fields)) {
        json_response(['error' => 'No fields to update'], 400);
    }
    $params[] = $id;
    $sql = 'UPDATE tasks SET ' . implode(', ', $fields) . ', updated_at = NOW() WHERE id = ?';
    $stmt = $pdo->prepare($sql);
    $stmt->execute($params);
    $task = fetch_task($pdo, $id);
    json_response(['task' => $task]);
}

function handle_delete(PDO $pdo): void {
    $id = 0;
    if (isset($_GET['id'])) {
        $id = (int)$_GET['id'];
    } else {
        $data = get_json_input();
        $id = isset($data['id']) ? (int)$data['id'] : 0;
    }
    if ($id <= 0) {
        json_response(['error' => 'Valid id is required'], 422);
    }
    $stmt = $pdo->prepare('DELETE FROM tasks WHERE id = ?');
    $stmt->execute([$id]);
    json_response(['success' => true]);
}

function fetch_task(PDO $pdo, int $id): array {
    $stmt = $pdo->prepare('SELECT id, title, is_completed, created_at, updated_at FROM tasks WHERE id = ?');
    $stmt->execute([$id]);
    $row = $stmt->fetch();
    if (!$row) {
        json_response(['error' => 'Task not found'], 404);
    }
    $row['is_completed'] = (bool)$row['is_completed'];
    return $row;
}

